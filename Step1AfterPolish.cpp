#include "ROOT/RDataFrame.hxx"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"

#include <cmath>
#include <iostream>
#include <TFile.h>
#include <TH1D.h>
#include <TMinuit.h>
#include <TRandom.h>
#include <vector>

// 全局变量，用于存储直方图和积分
TH1D* gHistSignal = nullptr;
TH1D* gHistBackground = nullptr;
double x1 = 0, x2 = 100;
double numofbin = 1000;
double step = (x2 - x1) / numofbin;

// 读取能量沉积数据
std::vector<Double_t> readEnergyDeposit(const std::string& filePath, const std::string& treeName, const std::string& branchName) {
    ROOT::RDataFrame df(treeName, filePath);
    return df.Take<Double_t>(branchName).GetValue();
    std::vector<Double_t> energyDepositVec;

    // df.Foreach([&energyDepositVec](Double_t EnergyDeposite) {
    //     energyDepositVec.push_back(EnergyDeposite);
    // },
    //            // mission accomplished
    //            {"EnergyDeposite"});

    // auto value = df.Take<Double_t>(branchName).GetValue();
    return energyDepositVec;
}

// 应用能量分辨率
std::vector<double> applyEnergyResolution(const std::vector<double>& edep, double resolution) {
    std::vector<double> random;
    for (double e : edep) {
        random.push_back(gRandom->Gaus(e, resolution * sqrt(2 * e)));
    }
    return random;
}
/*RInterface< Proxied, DS_t > ROOT::RDF::RInterface< Proxied, DataSource >::Redefine	(	std::string_view 	name,
std::string_view 	expression
)	*/

// 创建能量分布直方图
TH1D createHistogram(const std::vector<double>& data, const std::string& name, int bins, double min, double max) {
    TH1D hist(name.c_str(), (name + " Histogram").c_str(), bins, min, max);
    for (double energy : data) {
        hist.Fill(energy);
    }
    return hist;
}

// 计算信噪比目标函数
double SNRFunction(int low, int high) {
    double a = x1 + step * low;
    double b = x1 + step * high;

    double signal = gHistSignal->Integral(low, high, "width");
    double background = gHistBackground->Integral(low, high, "width");

    // 计算信噪比
    double SNR = signal / sqrt(background + signal);
    return SNR;
}

// int main() {
void Step1AfterPolish() {
    const std::string filePath = "/home/zhangjngyi/G4Code/newpdrsim2/build/.root";
    const std::string treeName1 = "EnergyDeposit";
    const std::string treeName2 = "EnergyDeposit2";
    const std::string branchName = "Edep";
    const double resolution = 0.075; // 能量分辨率
    const int bins = 1000;           // 直方图的分组数
    const double min = 0.0;          // 直方图的最小值
    const double max = 100.0;        // 直方图的最大值
    const char* CrystalScintillator = "CrystalScintillator";
    const char* EnergyDeposit = "EnergyDeposit";

    // 读取能量沉积数据
    std::vector<double> edep1 = readEnergyDeposit("/home/zhangjngyi/G4Code/newpdrsim2/build/test.root", CrystalScintillator, EnergyDeposit);  // 信号
    std::vector<double> edep2 = readEnergyDeposit("/home/zhangjngyi/G4Code/newpdrsim2/build/test2.root", CrystalScintillator, EnergyDeposit); // 背景

    // 应用能量分辨率
    std::vector<double> random1 = applyEnergyResolution(edep1, 0.075);
    std::vector<double> random2 = applyEnergyResolution(edep2, 0.075);

    // 创建直方图
    TH1D histSignal = createHistogram(random1, "Signal", bins, min, max);
    TH1D histBackground = createHistogram(random2, "Background", bins, min, max);
    // 画信号
    //  创建一个画布
    TCanvas canvas("canvas", "Signal Histogram", 800, 600);
    // 绘制直方图
    histSignal.SetLineColor(kBlue); // 设置线条颜色
    histSignal.SetLineWidth(2);     // 设置线条宽度
    histSignal.Draw();              // 绘制直方图
    // 保存为图像文件
    canvas.SaveAs("histSignal.png"); // 保存为 PNG 格式
                                     // 画本底
    // 创建一个画布
    TCanvas canvas2("canvas2", "Background Histogram", 800, 600);
    // 绘制直方图
    histBackground.SetLineColor(kRed); // 设置线条颜色
    histBackground.SetLineWidth(2);    // 设置线条宽度
    histBackground.Draw();             // 绘制直方图
    // 保存为图像文件
    canvas2.SaveAs("histBackground.png"); // 保存为 PNG 格式

    // 设置全局直方图变量
    gHistSignal = &histSignal;
    gHistBackground = &histBackground;

    int i, j;
    double lbound, ubound;
    double SNR = 0.0;
    for (i = 0; i < numofbin; i++) {
        for (j = i; j < numofbin; j++) {
            double tepSNR = SNRFunction(i, j);
            double teplbound = x1 + step * i;
            double tepubound = x1 + step * j;
            if (tepSNR > SNR) {
                lbound = teplbound;
                ubound = tepubound;
                SNR = tepSNR;
            } else {
                continue;
            }
        }
    }
    // 输出i,j,SNR
    std::cout << "lbound=" << lbound << " ubound=" << ubound << " SNR=" << SNR << std::endl;

    // 使用 TMinuit 优化
    // TMinuit minuit(2); // 两个参数 a 和 b
    // minuit.SetFCN(SNRFunction);

    // // 设置初值和范围
    // double startA = 0.2, stepA = 0.01; // 初值和步长
    // double startB = 0.8, stepB = 0.01;
    // minuit.DefineParameter(0, "a", startA, stepA, min, max); // 起始点参数
    // minuit.DefineParameter(1, "b", startB, stepB, min, max); // 终止点参数

    // std::cout << "Optimized Range: [" << a << ", " << b << "]" << std::endl;

    // 保存直方图到文件
    // TFile file("step1_with_minuit.root", "RECREATE");
    // histSignal.Write();
    // histBackground.Write();
    // file.Close();
}
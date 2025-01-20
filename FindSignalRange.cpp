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
double x1 = 0, x2 = 40;
double numofbin = 4000;
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
// 积分函数
double integralforbin(TH1D* hist, int low, int high) {
    double integral = 0;
    integral = gHistSignal->Integral(low, high, "width");
    return integral;
}

void FindSignalRange() {
    const std::string filePath = "/home/zhangjngyi/G4Code/newpdrsim2/build/.root";
    const std::string treeName1 = "EnergyDeposit";
    const std::string treeName2 = "EnergyDeposit2";
    const std::string branchName = "Edep";
    const double resolution = 0.075; // 能量分辨率
    const int bins = numofbin;       // 直方图的分组数
    const double min = x1;           // 直方图的最小值
    const double max = x2;           // 直方图的最大值
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
    canvas.SaveAs("histSignal.root"); // 保存为root格式
                                      // 画本底
    // 创建一个画布
    TCanvas canvas2("canvas2", "Background Histogram", 800, 600);
    // 绘制直方图
    histBackground.SetLineColor(kRed); // 设置线条颜色
    histBackground.SetLineWidth(2);    // 设置线条宽度
    histBackground.Draw();             // 绘制直方图
    // 保存为图像文件
    canvas2.SaveAs("histBackground.root"); // 保存为 root 格式

    // 设置全局直方图变量
    gHistSignal = &histSignal;
    gHistBackground = &histBackground;

    // 积分
    int i = 0;
    double r = 0;
    double integralofsignal = 0;
    for (i = 1000; i < numofbin; i++) {
        double temintegralofsignal = integralforbin(gHistSignal, 1000, x1 + 100 * i * step);
        if (temintegralofsignal != integralofsignal) {
            integralofsignal = temintegralofsignal;
            continue;
        } else {
            r = i * step;
            break;
        }
    }
    // 输出bound
    std::cout << "bound is " << r << std::endl;
}
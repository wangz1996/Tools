#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <random>
#include <unordered_map>
#include <utility>
#include <mutex>
#include <semaphore>
#include <future>
#include "json.hpp"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TRandom3.h"
#include "TMath.h"
using json = nlohmann::json;

std::mutex map_mutex;
std::counting_semaphore<20> semaphore(20);

std::unordered_map<int, float> rig_table = {{0, 2.04174}, {1, 2.29087}, {2, 2.5704}, {3, 2.88403}, {4, 3.23594}, {5, 3.63078}, {6, 4.0738}, {7, 4.57088}, {8, 5.12861}, {9, 5.7544}, {10, 6.45654}, {11, 7.24436}, {12, 8.12831}, {13, 9.12011}, {14, 10.2329}, {15, 11.4815}, {16, 12.8825}, {17, 14.4544}, {18, 16.2181}, {19, 18.197}, {20, 20.4174}, {21, 22.9087}, {22, 25.704}, {23, 28.8403}, {24, 32.3594}, {25, 36.3078}, {26, 40.738}, {27, 45.7088}, {28, 51.2861}, {29, 57.544}, {30, 64.5654}, {31, 72.4436}, {32, 81.2831}, {33, 91.2011}};
struct PairHash {
    std::size_t operator()(const std::pair<float, float>& p) const {
        // 使用 std::hash<float>，然后组合两个哈希值
        std::size_t h1 = std::hash<float>{}(p.first);
        std::size_t h2 = std::hash<float>{}(p.second);
        return h1 ^ (h2 << 1);  // 或者使用其它组合方式
    }
};

const std::string test_fname = "/home/wangzhen/mnt2/VLASTP/cutoff/database/small/result_lon_90_lat_0_9.json";
std::vector<double> sample_particle_energy(double E_min, double E_max, size_t size) {
    // 创建随机数生成器
    std::random_device rd;  // 用于获取随机种子
    std::mt19937 gen(rd()); // Mersenne Twister 随机数引擎
    std::uniform_real_distribution<> dis(0.0, 1.0); // 均匀分布在 [0, 1]

    std::vector<double> energies;
    energies.reserve(size); // 预分配内存以提高性能

    for (size_t i = 0; i < size; ++i) {
        double u = dis(gen); // 生成均匀分布的随机数
        // 使用逆累积分布函数计算能量
        double E = E_min * std::pow(1.0 - u, -1.0 / 1.7);
        // 确保能量在 [E_min, E_max] 范围内
        E = std::clamp(E, E_min, E_max);
        energies.push_back(E);
    }

    return energies;
}
bool fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

int main(int argc,char** argv){
	float rig;
	float longitude;
	float latitude;
	int is_allowed;
	float theta;
	float arc;
	constexpr int N_RIG=34;
	std::unordered_map<std::pair<float,float>,std::vector<float>,PairHash> umap_thetaarc_allow;
	std::unordered_map<std::pair<float,float>,std::vector<float>,PairHash> umap_thetaarc_numerator;
	std::unordered_map<std::pair<float,float>,std::vector<float>,PairHash> umap_thetaarc_denominator;
	//Init
	std::ifstream infile_init(test_fname);
	json data_test;
	infile_init >> data_test;
	for (const auto &item : data_test["allowed_data"]){
		float theta = item["theta"];
		float arc = item["arc"];
		umap_thetaarc_allow[{theta,arc}] = std::vector<float>(34,0.);
		umap_thetaarc_numerator[{theta,arc}] = std::vector<float>(34,0.);
		umap_thetaarc_denominator[{theta,arc}] = std::vector<float>(34,0.);
	}
	//Read a list of database file
	std::string flist = std::string(argv[1]);
	std::ifstream infile(flist);
	std::string fname;
	auto f = [&umap_thetaarc_allow, &umap_thetaarc_numerator, &umap_thetaarc_denominator](const std::string &fname)
	{
		// std::cout<<fname<<std::endl;
		float rig;
		float longitude;
		float latitude;
		int is_allowed;
		float theta;
		float arc;
		std::ifstream infile(fname);
		json data;
		infile >> data;
		std::string filename(fname);
		filename = filename.substr(filename.find_last_of('_') + 1);
		filename = filename.substr(0, filename.find_first_of('.'));
		int index = std::stoi(filename);
		for (const auto &item : data["allowed_data"])
		{
			longitude = item["longitude"];
			latitude = item["latitude"];
			is_allowed = item["is_allowed"];
			theta = item["theta"];
			arc = item["arc"];
			std::lock_guard<std::mutex> lock(map_mutex);
			umap_thetaarc_numerator[{theta, arc}][index] += is_allowed ? 1. : 0.;
			umap_thetaarc_denominator[{theta, arc}][index] += 1.;
		}
		semaphore.release();
	};
	std::vector<std::future<void>> futures;
	while (infile >> fname)
	{
		if (!fileExists(fname))
		{
			std::cout << "File not found: " << fname << std::endl;
			return 1;
		}
		semaphore.acquire(); // 阻塞直到有空闲线程槽
		futures.emplace_back(std::async(std::launch::async, f, fname));
	}
	// 等待所有线程完成
	for (auto &fut : futures)
	{
		fut.wait(); // 或 fut.get()
	}
	for(auto& it:umap_thetaarc_allow){
		for(int i=0;i<it.second.size();i++){
			it.second[i] = umap_thetaarc_numerator[it.first][i] / umap_thetaarc_denominator[it.first][i];
			std::cout<<umap_thetaarc_numerator[it.first][i]<<" "<<umap_thetaarc_denominator[it.first][i]<<std::endl;
		}
	}
	for(auto it:umap_thetaarc_allow){
		std::cout<<it.first.first<<" "<<it.first.second<<" ";
		for(auto i:it.second){
			std::cout<<i<<" ";
		}
		std::cout<<std::endl;
	}
	std::vector<double> ew = sample_particle_energy(0.1, 100, 10000000);
	TRandom3 rand(1234);
	TFile *fout = new TFile("equator_fullangle.root","recreate");
	int i_ta=0;
	for(auto it:umap_thetaarc_allow){
		TH1D *he=new TH1D(TString::Format("h%d",i_ta),"test",100,0,100);
		he->SetDirectory(0);
		TGraph *ge=new TGraph();
		ge->SetName(TString::Format("g%d",i_ta));
		auto vec = it.second;
		
		bool iszero = std::all_of(vec.begin(),vec.end(),[](int x){return x==0;});
		
		if(iszero)continue;
		// std::cout<<i_ta<<" "<<it.first.first<<" "<<it.first.second<<" ";
		// std::cout<<std::endl;
		for(int i=0;i<vec.size();i++){
			ge->SetPoint(ge->GetN(),rig_table[i],vec[i]);
		}
		int ntrial=0;
		while(he->GetEntries()<100000){
			double E = rand.Uniform(0.1,100);
			if(ge->Eval(E)<=0.)continue;
			double weight = TMath::Power(E,-2.7) * ge->Eval(E);
			if(weight<0.){
				std::cout<<"Negative weight: "<<TMath::Power(E,-2.7)<<" "<<ge->Eval(E)<<std::endl;
			}
			he->Fill(E,weight);
		}
		
		fout->cd();
		for(int i=1;i<=he->GetNbinsX();i++){
			if(he->GetBinContent(i)<0.){
				std::cout<<i_ta<<std::endl;
			}
		}
		he->Write();
		ge->Write();
		i_ta++;
		delete ge;
		// break;
	}
	fout->Close();
	return 0;
}

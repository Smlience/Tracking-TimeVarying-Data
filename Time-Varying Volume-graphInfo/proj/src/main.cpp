#include "package/ColorVolTF/ColorVolTF_MainWin.h"
#include "package/ColorVolTF/ColorSimple_MainWin.h"
#include "package/VarSelect/VarSelect_MainWin.h"
#include "ui/MainWin.h"
#include <QApplication>
#include <iostream>
using namespace std;
//#include <vld.h>

//////////////////////////////////////////////////////////////////////////
#include "render/VolumeRender.h"
void loadTestData(VolumeRender* render)
{
	//////////////////////////////////////////////////////////////////////////
	// for test
	// 彩色数据
	//render->loadData("./Data/Color/ColorSphere.vifo");
	//render->loadData("Z:/Foot/iFoot.vifo");
	//render->loadData("Y:/test/iFoot.vifo");
	//render->loadData("Y:/Foot_278x740x178/iFoot.vifo");
	//render->loadData("Y:/Foot_139x370x89/iFoot.vifo");
	//render->loadData("Z:/leg_256_256_282_white_salt_pepper_0.1.vifo");
	//render->loadData("Z:/leg_256_256_282.vifo");
	//论文用图2
	//render->loadData("E:/旧硬盘转移内容/Data/VolumeData/Photographic/胸腔/torso_512_256_256.vifo");
	//render->loadData("Y:/iFoot.vifo");

	// For color mapping compare
	//render->loadData("Z:/mapping_vs/abdomenSlice.vifo");
	//render->loadData("Z:/mapping_vs/brainSlice.vifo");


	// For gradient compare
	//render->loadData("Z:/grad_vs/dir_vs/ColorCube2.vifo");
	//render->loadData("Z:/head/ihead_256_256_256.vifo");
	//render->loadData("Z:/head/ihead_256_256_256_white_salt_pepper_0.1.vifo");
	//render->loadData("Z:/head/ihead_256_256_256_white_salt_pepper_0.05.vifo");
	//render->loadData("Z:/head/ihead_256_256_256_white_salt_pepper_SNR0.9.vifo");

	//render->loadData("E:/colorWind/ColorWind01.vifo");
	//render->loadData("Z:/ColorCube.vifo");
	//render->loadData("Z:/ihead_128_128_128.vifo");
	//render->loadData("Z:/ihead_256_256_256.vifo");
	//render->lodData("Z:/torso_512_256_256.vifo");
	//render->loadData("Z:/WhiteSphere.vifo");
	//render->loadData("Z:/TestSphere.vifo");
	//render->loadData("Z:/ColorCube1_Gauss.vifo");
	//render->loadData("./Data/Color/leg_256_256_282_white_salt_pepper_0.1.vifo");
	//render->loadData("./Data/Color/leg_256_256_282_salt_pepper_0.1.vifo");
	//render->loadData("M:/User_Easterman/Documents/Visual Studio 2008/Projects/KY2012/ColorVolTF/Data/Monkey-PET.vifo");
	//render->loadData("M:/User_Easterman/Documents/Visual Studio 2008/Projects/KY2012/ColorVolTF/Data/Virgo.vifo");
	//render->loadData("M:/User_Easterman/Documents/Visual Studio 2008/Projects/KY2012/ColorVolTF/Data/vhp-male-head-208x256x259-1-cuon.vifo");
	//render->loadData("M:/User_Easterman/Documents/Visual Studio 2008/Projects/KY2012/ColorVolTF/Data/vhp-male-head-208x256x259-2-cuon.vifo");

	// 标量数据
	//render->loadData("K:/Data/VolumeData/head.vifo");
	//render->loadData("K:/Data/VolumeData/engine.vifo");

	//render->loadData("data/TOOTH_8bit_128_128_160.vifo");
	render->loadData("E:\\projects\\visual c++_projects\\vs2012\\Time-Varying Volume-graphInfo/vortex/Uf.vifo");
	//G:\vortex
	//render->loadData("data/char/Uf.vifo");

	//render->loadData("Y:/head.vifo");
	//render->loadData("K:/Data/VolumeData/TOOTH_8bit_120_120_161.vifo");
	//render->loadData("K:/Data/VolumeData/VisMale.vifo");
	//render->loadData("K:/Data/VolumeData/Hurricane/Hurricane.vifo");
	//render->loadData("K:/Data/VolumeData/bonsai256x256x256.vifo");
	//render->loadData("./data/test.vifo");
}

//////////////////////////////////////////////////////////////////////////

// App
// #define APP ColorVolTF_MainWin
// #define APP ColorSimple_MainWin
#define APP VarSelect_MainWin
// #define APP MainWin
int main(int argc, char *argv[])
{
// 	Volume v;
// 	v.loadVolume_process("data/char/Uf.vifo");
	QApplication a(argc, argv);
	CREATE_APP_WIN(APP);
	return a.exec();
//	return 0;
}

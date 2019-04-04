#include "PAConfigSolutionDlg.h"

#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QImage>
#include <QGridLayout>
#include <QFileDialog>
#include <QFile>


#include <stdio.h>
#include <string.h>
#include <windows.h>

#pragma warning (disable: 4996)

PAConfigSolutionDlg::PAConfigSolutionDlg(QWidget* parent) : QDialog(parent)
{
	setWindowTitle(tr("Create PA Inspect Solution"));
	
	//初始化一些变量
	m_IsConfig = false;

	//探头信息
	probe.m_ProbeID = 121;
	strcpy(probe.m_ProbeName, "5L64-A2");
	probe.m_Height = 5;
	probe.m_Pitch = 0.6;
	probe.m_ElementNum = 64;
	probe.m_Frequency = 5;
	probe.m_FirstConnection = 1;

	//楔块信息
	wedge.m_WedgeID = 544;
	strcpy(wedge.m_WedgeName, "SA2-N552-IHC dual 5L64");
	//无楔块
	wedge.m_WedgeAngle = 0;
	wedge.m_h1 = 0;
	wedge.m_ProbeOffsetX = 0;
	wedge.m_ProbeOffsetY = 0;
	wedge.m_h2 = 0;
	wedge.m_w1 = 68.53;
	wedge.m_Velocity = 2330;

	//扫查信息
	ScanInfo scan;
	scan.m_Type = ScanType::ePAAngle;
	scan.m_VelocityType = VelocityType::eLongitudinal;
	scan.m_Reflect = 0;
	scan.m_ChannelSize = 64;
	scan.m_ChannelNum = 91;
	scan.m_AngleStart = -45;
	scan.m_AngleStop = 45;
	scan.m_ElementStart = 1;
	scan.m_ElementStop = 1;
	scan.m_SendFocus = 30;
	scan.m_RecvFocusStart = 10;
	scan.m_RecvFocusEnd = 60;
	scan.m_RecvFocusNum = 11;
	scan.m_FocalType = 3;
	int angleStep = 1;
	int elementStep = 1;
	
	//超声信息
	AGain = 30;
	DGain = 0;
	pulseWidth = 1 / (probe.m_Frequency * 2);
	divisor = 1;

	encoder.m_ScanStart = encoder.m_IndexStart = 0;
	encoder.m_ScanRange = encoder.m_IndexRange = 0;
	encoder.m_ScanStep = encoder.m_IndexStep =  1;
	encoder.m_ScanPointNum = (encoder.m_ScanRange / encoder.m_ScanStep) + 1;
	encoder.m_IndexPointNum = (encoder.m_IndexRange / encoder.m_IndexStep) + 1;
	encoder.m_EncoderResolution[0] = encoder.m_EncoderResolution[1] = 200;
	encoder.m_EncoderPolarity[0] = encoder.m_EncoderPolarity[1] = 0;
	encoder.m_ScanAxisID = 0;
	
	//材料信息
	strcpy(material.m_MaterialName, "Steel");
	material.m_ShearVelocity = 3150;
	material.m_LongitudinalVelocity = 5810;
	material.m_Thickness = 50;

	// ********************* 选项卡 *******************
	QTabWidget* TabWidget = new QTabWidget(this);
	TabWidget->setStyleSheet("QTabBar::tab{width:200; font:10pt 'consolas'}");
	
	

	//***************************** 选项卡第一个页面 Probe and Wedge *******************************
	//探头GroupBox
	QGroupBox* probeBox = new QGroupBox(tr("Probe"), this);
	
	//探头高度
	QLabel* ProbeHeight = new QLabel(tr("Probe Height(cm)"), this);
	m_ProbeHeight = new QLineEdit(this);
	m_ProbeHeight->setMaximumWidth(150);
	m_ProbeHeight->setText(QString::number(probe.m_Height, 'f', 1));
	//阵元间距
	QLabel* ProbePitch = new QLabel("Probe Pitch(mm)", this);
	m_ProbePitch = new QLineEdit(this);
	m_ProbePitch->setMaximumWidth(150);
	m_ProbePitch->setText(QString::number(probe.m_Pitch, 'f', 1));
	//阵元数量
	QLabel* ElementNum = new QLabel(tr("Element Num"), this);
	m_ProbeElementNum = new QLineEdit(this);
	m_ProbeElementNum->setMaximumWidth(150);
	m_ProbeElementNum->setText(QString::number(probe.m_ElementNum, 'f', 0));
	//探头频率
	QLabel* Frequency = new QLabel(tr("Frequency(MHz)"), this);
	m_ProbeFrequency = new QLineEdit(this);
	m_ProbeFrequency->setMaximumWidth(150);
	m_ProbeFrequency->setText(QString::number(probe.m_Frequency, 'f', 1));

	//探头图例
	QLabel* imageProbe = new QLabel(this);
	imageProbe->setMaximumHeight(300);
	imageProbe->setMaximumWidth(350);

	QGridLayout* probeBoxLayout = new QGridLayout(this);
	probeBoxLayout->addWidget(imageProbe, 0, 0, 4, 2);
	probeBoxLayout->addWidget(ProbeHeight, 0, 2, 1, 1);
	probeBoxLayout->addWidget(m_ProbeHeight, 0, 3, 1, 1);
	probeBoxLayout->addWidget(ProbePitch, 1, 2, 1, 1);
	probeBoxLayout->addWidget(m_ProbePitch, 1, 3, 1, 1);
	probeBoxLayout->addWidget(ElementNum, 2, 2, 1, 1);
	probeBoxLayout->addWidget(m_ProbeElementNum, 2, 3, 1, 1);
	probeBoxLayout->addWidget(Frequency, 3, 2, 1, 1);
	probeBoxLayout->addWidget(m_ProbeFrequency, 3, 3, 1, 1);
	probeBoxLayout->setHorizontalSpacing(5);
	probeBox->setLayout(probeBoxLayout);

	//楔块（水）界面groupBox
	QGroupBox* wedgeBox = new QGroupBox(tr("Wedge"), this);

	QLabel* wedgeAngle = new QLabel(tr("Wedge Angle(deg)"), this);
	m_WedgeAngle = new QLineEdit(this);
	m_WedgeAngle->setMaximumWidth(150);
	m_WedgeAngle->setText(QString::number(wedge.m_WedgeAngle, 'f', 1));

	QLabel* ProbeOffsetY = new QLabel(tr("Offset H(mm)"), this);
	m_ProbeOffsetY = new QLineEdit(this);
	m_ProbeOffsetY->setMaximumWidth(150);
	m_ProbeOffsetY->setText(QString::number(wedge.m_ProbeOffsetY, 'f', 1));

	QLabel* WedgeVelocity = new QLabel(tr("Wedge Velocity(m/s)"), this);
	m_WedgeVelocity = new QLineEdit(this);
	m_WedgeVelocity->setMaximumWidth(150);
	m_WedgeVelocity->setText(QString::number(wedge.m_Velocity, 'f', 1));

	//楔块图例
	QLabel* imageWedge = new QLabel(this);
	imageWedge->setMaximumHeight(300);
	imageWedge->setMaximumWidth(350);

	QGridLayout* wedgeBoxLayout = new QGridLayout(this);
	wedgeBoxLayout->addWidget(imageWedge, 0, 0, 3, 2);
	wedgeBoxLayout->addWidget(wedgeAngle, 0, 2, 1, 1);
	wedgeBoxLayout->addWidget(m_WedgeAngle, 0, 3, 1, 1);	
	wedgeBoxLayout->addWidget(ProbeOffsetY, 1, 2, 1, 1);
	wedgeBoxLayout->addWidget(m_ProbeOffsetY, 1, 3, 1, 1);
	wedgeBoxLayout->addWidget(WedgeVelocity, 2, 2, 1, 1);
	wedgeBoxLayout->addWidget(m_WedgeVelocity, 2, 3, 1, 1);
	wedgeBoxLayout->setHorizontalSpacing(5);
	wedgeBox->setLayout(wedgeBoxLayout);

	QWidget* ProbeandWedgePage = new QWidget(this);
	QVBoxLayout* PWPageLayout = new QVBoxLayout(this);	
	PWPageLayout->addWidget(probeBox);
	PWPageLayout->addWidget(wedgeBox);
	ProbeandWedgePage->setLayout(PWPageLayout);

	TabWidget->addTab(ProbeandWedgePage, "Probe and Wedge");

	// **********************************************************************************************

	//***********************************选项卡第二个界面 UT and Material*********************************
	//UT GroupBox
	QGroupBox* HardwareBox = new QGroupBox("UT", this);

	QLabel* AnalogGain = new QLabel(tr("Analog Gain(dB)"), this);
	m_AGain = new QLineEdit(this);
	m_AGain->setMaximumWidth(150);
	m_AGain->setText(QString::number(AGain, 'f', 1));
	
	QLabel* DigtalGain = new QLabel(tr("DigtalGain(dB)"), this);
	m_DGain = new QLineEdit(this);
	m_DGain->setMaximumWidth(150);
	m_DGain->setText(QString::number(DGain, 'f', 1));

	QLabel* PulseWidth = new QLabel(tr("PulseWidth(ns)"), this);
	m_PulseWidth = new QLineEdit(this);
	m_PulseWidth->setMaximumWidth(150);
	m_PulseWidth->setText(QString::number(pulseWidth * 1e3, 'f', 0));

	QGridLayout* HardwareBoxLayout = new QGridLayout(this);
	HardwareBoxLayout->addWidget(AnalogGain, 0, 0, 1, 1);
	HardwareBoxLayout->addWidget(m_AGain, 0, 1, 1, 1);
	HardwareBoxLayout->addWidget(DigtalGain, 1, 0, 1, 1);
	HardwareBoxLayout->addWidget(m_DGain, 1, 1, 1, 1);
	HardwareBoxLayout->addWidget(PulseWidth, 2, 0, 1, 1);
	HardwareBoxLayout->addWidget(m_PulseWidth, 2, 1, 1, 1);
	HardwareBox->setLayout(HardwareBoxLayout);

	//Material GroupBox
	QGroupBox* materialBox = new QGroupBox(tr("Material"), this);
	//材料名称
	QLabel* Name = new QLabel(tr("Name"), this);
	m_MaterialName = new QLineEdit(this);
	m_MaterialName->setMaximumWidth(150);
	m_MaterialName->setText(material.m_MaterialName);
	//材料纵波声速
	QLabel* SVelocity = new QLabel(tr("SVelocity(m/s)"), this);
	m_ShearVelocity = new QLineEdit(this);
	m_ShearVelocity->setMaximumWidth(150);
	m_ShearVelocity->setText(QString::number(material.m_ShearVelocity, 'f', 1));
	//材料横波声速
	QLabel* LVelocity = new QLabel(tr("LVelocity(m/s)"), this);
	m_LongitudinalVelocity = new QLineEdit(this);
	m_LongitudinalVelocity->setMaximumWidth(150);
	m_LongitudinalVelocity->setText(QString::number(material.m_LongitudinalVelocity, 'f', 1));
	//材料厚度
	QLabel* Thickness = new QLabel(tr("Thickness(mm)"), this);
	m_Thickness = new QLineEdit(this);
	m_Thickness->setMaximumWidth(150);
	m_Thickness->setText(QString::number(material.m_Thickness, 'f', 1));

	QGridLayout* materialBoxLayout = new QGridLayout(this);
	materialBoxLayout->addWidget(Name, 0, 0, 1, 1);
	materialBoxLayout->addWidget(m_MaterialName, 0, 1, 1, 1);
	materialBoxLayout->addWidget(SVelocity, 1, 0, 1, 1);
	materialBoxLayout->addWidget(m_ShearVelocity, 1, 1, 1, 1);
	materialBoxLayout->addWidget(LVelocity, 2, 0, 1, 1);
	materialBoxLayout->addWidget(m_LongitudinalVelocity, 2, 1, 1, 1);
	materialBoxLayout->addWidget(Thickness, 3, 0, 1, 1);
	materialBoxLayout->addWidget(m_Thickness, 3, 1, 1, 1);
	materialBox->setLayout(materialBoxLayout);

	//新建一个QWidget放置两个GroupBox
	QWidget* UTandMaterialPage = new QWidget(this);
	QVBoxLayout* UMPageLayout = new QVBoxLayout(this);
	UMPageLayout->addWidget(HardwareBox);
	UMPageLayout->addWidget(materialBox);
	UTandMaterialPage->setLayout(UMPageLayout);

	//加到选项卡中
	TabWidget->addTab(UTandMaterialPage, "UT and Material");

	//**************************************************************************************************
	
	//************************************** 选项卡第三个页面 *******************************************
	//扫查信息
	QGroupBox* scanBox = new QGroupBox(this);

	QLabel* Type = new QLabel(tr("Scan Type"), this);
	m_ScanType = new QComboBox(this);
	
	m_ScanType->addItem(tr("Angle"));
	m_ScanType->addItem(tr("Linear"));

	QLabel* VelocityType = new QLabel(tr("Velocity Type"), this);
	m_VelocityType = new QComboBox(this);
	m_VelocityType->addItem(tr("Longitudinal"));
	m_VelocityType->addItem(tr("Shear"));

	QLabel* ReflectNum = new QLabel(tr("Reflect Number"), this);
	m_Reflect = new QLineEdit(this);
	m_Reflect->setMaximumWidth(150);
	m_Reflect->setText(QString::number(scan.m_Reflect, 'f', 0));

	QLabel* ChannelSize = new QLabel(tr("Channel Size"), this);
	m_ChannelSize = new QLineEdit(this);
	m_ChannelSize->setMaximumWidth(150);
	m_ChannelSize->setText(QString::number(scan.m_ChannelSize, 'f', 0));

	QLabel* AngleStart = new QLabel(tr("Angle Start"), this);
	m_AngleStart = new QLineEdit(this);
	m_AngleStart->setMaximumWidth(150);
	m_AngleStart->setText(QString::number(scan.m_AngleStart, 'f', 1));

	QLabel* AngleStop = new QLabel(tr("Angle Stop"), this);
	m_AngleStop = new QLineEdit(this);
	m_AngleStop->setMaximumWidth(150);
	m_AngleStop->setText(QString::number(scan.m_AngleStop, 'f', 1));

	QLabel* AngleStep = new QLabel(tr("Angle Step"), this);
	m_AngleStep = new QLineEdit(this);
	m_AngleStep->setMaximumWidth(150);
	m_AngleStep->setText(QString::number(angleStep, 'f', 1));

	QLabel* ElementStart = new QLabel(tr("Element Start"), this);
	m_ElementStart = new QLineEdit(this);
	m_ElementStart->setMaximumWidth(150);
	m_ElementStart->setText(QString::number(scan.m_ElementStart, 'f', 0));

	QLabel* ElementStop = new QLabel(tr("Element Stop"), this);
	m_ElementStop = new QLineEdit(this);
	m_ElementStop->setReadOnly(true);
	m_ElementStop->setMaximumWidth(150);
	m_ElementStop->setText(QString::number(scan.m_ElementStop, 'f', 0));
	m_ElementStop->setReadOnly(true);

	QLabel* ElementStep = new QLabel(tr("Element Step"), this);
	m_ElementStep = new QLineEdit(this);
	m_ElementStep->setMaximumWidth(150);
	m_ElementStep->setText(QString::number(elementStep, 'f', 0));
	m_ElementStep->setReadOnly(true);

	QLabel* SendFocus = new QLabel(tr("Send Focus(mm)"), this);
	m_SendFocus = new QLineEdit(this);
	m_SendFocus->setMaximumWidth(150);
	m_SendFocus->setText(QString::number(scan.m_SendFocus, 'f', 1));

	QLabel* RecvFocusStart = new QLabel(tr("Receive Start(mm)"), this);
	m_RecvFocusStart = new QLineEdit(this);
	m_RecvFocusStart->setMaximumWidth(150);
	m_RecvFocusStart->setText(QString::number(scan.m_RecvFocusStart, 'f', 1));

	QLabel* RecvFocusEnd = new QLabel(tr("Receive End(mm)"), this);
	m_RecvFocusEnd = new QLineEdit(this);
	m_RecvFocusEnd->setMaximumWidth(150);
	m_RecvFocusEnd->setText(QString::number(scan.m_RecvFocusEnd, 'f', 1));

	QLabel* RecvFocusStep = new QLabel(tr("Receive Step(mm)"), this);
	m_RecvFocusStep = new QLineEdit(this);
	m_RecvFocusStep->setMaximumWidth(150);

	double recStep = (scan.m_RecvFocusNum == 1) ? 10 : (scan.m_RecvFocusEnd - scan.m_RecvFocusStart) / (scan.m_RecvFocusNum - 1);

	m_RecvFocusStep->setText(QString::number(recStep, 'f', 1));

	QLabel* FocalType = new QLabel(tr("Focal Type"), this);
	m_FocalType = new QComboBox(this);
	m_FocalType->setMaximumWidth(150);
	m_FocalType->addItem(tr("vertical"));
	m_FocalType->addItem(tr("Horizon"));
	m_FocalType->addItem(tr("arc"));
	m_FocalType->setCurrentIndex(scan.m_FocalType - 1);

	QGridLayout* scanBoxLayout = new QGridLayout(this);
	//第一行
	scanBoxLayout->addWidget(Type, 0, 0, 1, 1);
	scanBoxLayout->addWidget(m_ScanType, 0, 1, 1, 1);
	scanBoxLayout->addWidget(VelocityType, 0, 2, 1, 1);
	scanBoxLayout->addWidget(m_VelocityType, 0, 3, 1, 1);
	scanBoxLayout->addWidget(ReflectNum, 1, 0, 1, 1);
	scanBoxLayout->addWidget(m_Reflect, 1, 1, 1, 1);
	scanBoxLayout->addWidget(ChannelSize, 1, 2, 1, 1);
	scanBoxLayout->addWidget(m_ChannelSize, 1, 3, 1, 1);
	//第二行
	scanBoxLayout->addWidget(AngleStart, 2, 0, 1, 1);
	scanBoxLayout->addWidget(m_AngleStart, 2, 1, 1, 1);
	scanBoxLayout->addWidget(AngleStop, 2, 2, 1, 1);
	scanBoxLayout->addWidget(m_AngleStop, 2, 3, 1, 1);
	scanBoxLayout->addWidget(AngleStep, 3, 0, 1, 1);
	scanBoxLayout->addWidget(m_AngleStep, 3, 1, 1, 1);
	//第三行
	scanBoxLayout->addWidget(ElementStart, 4, 0, 1, 1);
	scanBoxLayout->addWidget(m_ElementStart, 4, 1, 1, 1);
	scanBoxLayout->addWidget(ElementStop, 4, 2, 1, 1);
	scanBoxLayout->addWidget(m_ElementStop, 4, 3, 1, 1);
	scanBoxLayout->addWidget(ElementStep, 5, 0, 1, 1);
	scanBoxLayout->addWidget(m_ElementStep, 5, 1, 1, 1);
	//第四行
	scanBoxLayout->addWidget(SendFocus, 6, 0, 1, 1);
	scanBoxLayout->addWidget(m_SendFocus, 6, 1, 1, 1);
	scanBoxLayout->addWidget(RecvFocusStart, 6, 2, 1, 1);
	scanBoxLayout->addWidget(m_RecvFocusStart, 6, 3, 1, 1);
	scanBoxLayout->addWidget(RecvFocusEnd, 7, 0, 1, 1);
	scanBoxLayout->addWidget(m_RecvFocusEnd, 7, 1, 1, 1);
	scanBoxLayout->addWidget(RecvFocusStep, 7, 2, 1, 1);
	scanBoxLayout->addWidget(m_RecvFocusStep, 7, 3, 1, 1);

	//第五行
	scanBoxLayout->addWidget(FocalType, 8, 0, 1, 1);
	scanBoxLayout->addWidget(m_FocalType, 8, 1, 1, 1);
	scanBox->setLayout(scanBoxLayout);

	//图片部分
	QGroupBox* bafBox = new QGroupBox(this);

	AngleScan = new QLabel(tr("Photo angle"), this);
	LinearScan = new QLabel(tr("Photo Linear"), this);
	vertical = new QLabel(tr("Photo vertical"), this);
	Horizon = new QLabel(tr("Photo Horizon"), this);
	arc = new QLabel(tr("Photo arc"), this);
	LinearScan->setVisible(false);
	vertical->setVisible(false);
	Horizon->setVisible(false);
	AngleScan->setMaximumSize(350, 300);
	LinearScan->setMaximumSize(350, 300);
	vertical->setMaximumSize(350, 300);
	Horizon->setMaximumSize(350, 300);
	arc->setMaximumSize(350, 300);

	QGridLayout* photolayout = new QGridLayout(this);
	photolayout->addWidget(AngleScan, 0, 0, 1, 1);
	photolayout->addWidget(LinearScan, 0, 0, 1, 1);
	photolayout->addWidget(vertical, 1, 0, 1, 1);
	photolayout->addWidget(Horizon, 1, 0, 1, 1);
	photolayout->addWidget(arc, 1, 0, 1, 1);

	QWidget* photowidget = new QWidget(this);
	photowidget->setLayout(photolayout);
	QHBoxLayout* BAFlayout = new QHBoxLayout(this);
	BAFlayout->addWidget(photowidget);
	BAFlayout->addWidget(scanBox);
	bafBox->setLayout(BAFlayout);

	QHBoxLayout* BAFMainlayout = new QHBoxLayout(this);
	BAFMainlayout->addWidget(bafBox);
	this->setLayout(BAFMainlayout);//把this放到最大的布局中，才能自适应大小

	QWidget* ScanPage = new QWidget(this);
	ScanPage->setLayout(BAFMainlayout);
	TabWidget->addTab(ScanPage, "Scan and Focus");

	//****************************************************************************************************//
	
	//************************************** 选项卡第四个页面 Encoder*****************************************
	//扫查轴ID 0-x轴，1-y轴
	QLabel* ScanAxisID = new QLabel(tr("ScanAxis ID"), this);
	m_pAxis = new QComboBox(this);
	m_pAxis->addItem("1", 0);
	m_pAxis->addItem("2", 1);
	m_pAxis->setCurrentIndex(encoder.m_ScanAxisID);

	//扫查轴
	QLabel* ScanAxis = new QLabel("1   ", this);
	//扫查轴编码器起点(mm)
	QLabel* Start1 = new QLabel(tr("Start(mm)"), this);
	m_pStart[0] = new QLineEdit(this);
	m_pStart[0]->setText(QString::number(encoder.m_ScanStart, 'f', 2));
	m_pStart[0]->setMaximumWidth(150);


	//扫查轴编码器范围(mm)
	QLabel* Range1 = new QLabel(tr("Range(mm)"), this);
	m_pRange[0] = new QLineEdit(this);
	m_pRange[0]->setText(QString::number(encoder.m_ScanRange, 'f', 2));
	m_pRange[0]->setMaximumWidth(150);

	//扫查轴编码器步进(mm)
	QLabel* Step1 = new QLabel(tr("Step(mm)"), this);
	m_pStep[0] = new QLineEdit(this);
	m_pStep[0]->setText(QString::number(encoder.m_ScanStep, 'f', 2));
	m_pStep[0]->setMaximumWidth(150);

	//扫查轴极性 0 -- 正向 1 -- 反向
	QLabel* Polarity1 = new QLabel(tr("Polarity"), this);
	m_pPolarity[0] = new QComboBox(this);
	m_pPolarity[0]->addItem(tr("Positive"), 0);
	m_pPolarity[0]->addItem(tr("Negative"), 1);
	m_pPolarity[0]->setCurrentIndex(encoder.m_EncoderPolarity[0]);

	//扫查轴分辨率
	QLabel* Resolution1 = new QLabel(tr("Resolution(mm)"), this);
	m_pResolution[0] = new QLineEdit(this);
	m_pResolution[0]->setText(QString::number(encoder.m_EncoderResolution[0], 'f', 0));
	m_pResolution[0]->setMaximumWidth(150);

	//步进轴
	QLabel* IndexAxis = new QLabel("2   ", this);
	//步进轴编码器起点(mm)
	QLabel* Start2 = new QLabel(tr("Start(mm)"), this);
	m_pStart[1] = new QLineEdit(this);
	m_pStart[1]->setText(QString::number(encoder.m_IndexStart, 'f', 2));
	m_pStart[1]->setMaximumWidth(150);

	//步进轴编码器范围(mm)
	QLabel* Range2 = new QLabel(tr("Range(mm)"), this);
	m_pRange[1] = new QLineEdit(this);
	m_pRange[1]->setText(QString::number(encoder.m_IndexRange, 'f', 2));
	m_pRange[1]->setMaximumWidth(150);

	//步进轴编码器步进(mm)
	QLabel* Step2 = new QLabel(tr("Step(mm)"), this);
	m_pStep[1] = new QLineEdit(this);
	m_pStep[1]->setText(QString::number(encoder.m_IndexStep, 'f', 2));
	m_pStep[1]->setMaximumWidth(150);

	//步进轴极性0 -- 正向 1 -- 反向
	QLabel* Polarity2 = new QLabel(tr("Polarity"), this);
	m_pPolarity[1] = new QComboBox(this);
	m_pPolarity[1]->addItem(tr("Positive"), 0);
	m_pPolarity[1]->addItem(tr("Negative"), 1);
	m_pPolarity[1]->setCurrentIndex(encoder.m_EncoderPolarity[1]);
	//步进轴分辨率
	QLabel* Resolution2 = new QLabel(tr("Resolution(mm)"), this);
	m_pResolution[1] = new QLineEdit(this);
	m_pResolution[1]->setText(QString::number(encoder.m_EncoderResolution[1], 'f', 0));
	m_pResolution[1]->setMaximumWidth(150);

	//Encoder图例
	QLabel* imageEncoder = new QLabel(this);
	imageEncoder->setMaximumHeight(300);
	imageEncoder->setMaximumWidth(350);
	
	QGridLayout* encoderBoxLayout = new QGridLayout(this);
	encoderBoxLayout->addWidget(imageEncoder, 0, 0, 7, 5);

	encoderBoxLayout->addWidget(ScanAxisID, 0, 7, 1, 1);
	encoderBoxLayout->addWidget(m_pAxis, 0, 8, 1, 1);

	encoderBoxLayout->addWidget(ScanAxis, 1, 5, 1, 1);
	encoderBoxLayout->addWidget(Start1, 1, 6, 1, 1);
	encoderBoxLayout->addWidget(m_pStart[0], 1, 7, 1, 1);
	encoderBoxLayout->addWidget(Range1, 1, 8, 1, 1);
	encoderBoxLayout->addWidget(m_pRange[0], 1, 9, 1, 1);
	encoderBoxLayout->addWidget(Step1, 2, 6, 1, 1);
	encoderBoxLayout->addWidget(m_pStep[0], 2, 7, 1, 1);
	encoderBoxLayout->addWidget(Resolution1, 2, 8, 1, 1);
	encoderBoxLayout->addWidget(m_pResolution[0], 2, 9, 1, 1);
	encoderBoxLayout->addWidget(Polarity1, 3, 6, 1, 1);
	encoderBoxLayout->addWidget(m_pPolarity[0], 3, 7, 1, 1);

	encoderBoxLayout->addWidget(IndexAxis, 4, 5, 1, 1);
	encoderBoxLayout->addWidget(Start2, 4, 6, 1, 1);
	encoderBoxLayout->addWidget(m_pStart[1], 4, 7, 1, 1);
	encoderBoxLayout->addWidget(Range2, 4, 8, 1, 1);
	encoderBoxLayout->addWidget(m_pRange[1], 4, 9, 1, 1);
	encoderBoxLayout->addWidget(Step2, 5, 6, 1, 1);
	encoderBoxLayout->addWidget(m_pStep[1], 5, 7, 1, 1);
	encoderBoxLayout->addWidget(Resolution2, 5, 8, 1, 1);
	encoderBoxLayout->addWidget(m_pResolution[1], 5, 9, 1, 1);
	encoderBoxLayout->addWidget(Polarity2, 6, 6, 1, 1);
	encoderBoxLayout->addWidget(m_pPolarity[1], 6, 7, 1, 1);

	QWidget* EncoderPage = new QWidget(this);
	EncoderPage->setLayout(encoderBoxLayout);
	TabWidget->addTab(EncoderPage, "Encoder");
	//***********************************************************************************************//

	//应用按钮
	m_pApply = new QPushButton(tr("Apply"), this);

	//浏览文件按钮
	m_pBrowse = new QPushButton(tr("Browse"), this);

	m_pFilePath = new QLineEdit(this);

	char result[MAX_PATH];
	std::string path(result, GetModuleFileName(NULL, result, MAX_PATH));
	auto pos = path.find("\\bin");
	path = path.substr(0, pos) + "\\resource\\inspect\\" + "DefaultSolution.xml";

	m_pFilePath->setText(path.c_str());

	//*************************************** 加载图片 **********************************************
	char Presult[MAX_PATH];
	std::string path0(Presult, GetModuleFileName(NULL, Presult, MAX_PATH));
	auto pos0 = path0.find("\\bin");
	path0 = path0.substr(0, pos0) + "/resource/image/pa/";

	//探头楔块页面图片
	QString photopath = QString::fromStdString(path0 + +"Probe.png");
	QImage* imgProbe = new QImage;
	imgProbe->load(photopath);
	imageProbe->setPixmap(QPixmap::fromImage(*imgProbe));
	imageProbe->setScaledContents(true);  //图片随label变化

	photopath = QString::fromStdString(path0 + +"Wedge.jpg");
	QImage* imgWedge = new QImage;
	imgWedge->load(photopath);
	imageWedge->setPixmap(QPixmap::fromImage(*imgWedge));
	imageWedge->setScaledContents(true);  //图片随label变化

	//声束聚焦页面图片
	photopath = QString::fromStdString(path0 + "AngleScan.png");
	QImage imgAngleScan(photopath);//读图片
	AngleScan->setPixmap(QPixmap::fromImage(imgAngleScan));//图片加到label上
	AngleScan->setScaledContents(true);//图片随label变化

	photopath = QString::fromStdString(path0 + "LinearScan.png");
	QImage imgLinearScan(photopath);//读图片
	LinearScan->setPixmap(QPixmap::fromImage(imgLinearScan));//图片加到label上
	LinearScan->setScaledContents(true);//图片随label变化

	photopath = QString::fromStdString(path0 + +"Vertical.png");
	QImage imgVertical(photopath);
	vertical->setPixmap(QPixmap::fromImage(imgVertical));
	vertical->setScaledContents(true);

	photopath = QString::fromStdString(path0 + +"Horizon.png");
	QImage imgHorizon(photopath);
	Horizon->setPixmap(QPixmap::fromImage(imgHorizon));
	Horizon->setScaledContents(true);

	photopath = QString::fromStdString(path0 + +"arc.png");
	QImage imgArc(photopath);
	arc->setPixmap(QPixmap::fromImage(imgArc));
	arc->setScaledContents(true);

	//编码器页面图片
	photopath = QString::fromStdString(path0 + +"Encoder.jpg");
	QImage* imgEncoder = new QImage;
	imgEncoder->load(photopath);
	imageEncoder->setPixmap(QPixmap::fromImage(*imgEncoder));
	imageEncoder->setScaledContents(true);//图片随label变化

	//************************************************************************************************

	QGridLayout* totalLayout = new QGridLayout(this);
	totalLayout->addWidget(TabWidget, 0, 0, 24, 12);
	totalLayout->addWidget(m_pBrowse, 24, 0, 1, 1);
	totalLayout->addWidget(m_pFilePath, 24, 1, 1, 11);
	totalLayout->addWidget(m_pApply, 25, 11, 1, 1);
	this->setLayout(totalLayout);

	connect(m_pApply, SIGNAL(clicked()), this, SLOT(apply()));
	connect(m_pBrowse, SIGNAL(clicked()), this, SLOT(browse()));
	connect(m_ScanType, SIGNAL(currentIndexChanged(int)), this, SLOT(modeChanged(int)));
	connect(m_AngleStart, SIGNAL(textChanged(const QString &)), this, SLOT(angle(const QString &)));
	connect(m_ElementStart, SIGNAL(textChanged(const QString &)), this, SLOT(linear(const QString &)));
	connect(m_FocalType, SIGNAL(currentIndexChanged(int)), this, SLOT(FocalmodeChanged(int)));
	this->resize(905, 762);
}

PAConfigSolutionDlg::~PAConfigSolutionDlg()
{
}

bool PAConfigSolutionDlg::isConfig()
{
	return m_IsConfig;
}

QString PAConfigSolutionDlg::getPath()
{
	return m_pFilePath->text();
}

void PAConfigSolutionDlg::setIsConfig(bool config)
{
	m_IsConfig = config;
}

void PAConfigSolutionDlg::browse()
{
	char result[MAX_PATH];
	std::string path(result, GetModuleFileName(NULL, result, MAX_PATH));
	auto pos = path.find("\\bin");
	path = path.substr(0, pos) + "\\resource\\inspect\\";
	QString file_name = QFileDialog::getSaveFileName(this, tr("Create PA Solution"), path.c_str(), "solution(*.xml)");

	if (!file_name.isNull())
	{
		m_pFilePath->setText(file_name);
	}
}

void PAConfigSolutionDlg::apply()
{
	//探头信息
	probe.m_ProbeID = m_ProbeID->text().toInt();
	QByteArray probeName = m_ProbeName->text().toLatin1(); 
	strcpy(probe.m_ProbeName, probeName.data());
	probe.m_Height = m_ProbeHeight->text().toDouble();
	probe.m_Pitch = m_ProbePitch->text().toDouble();
	probe.m_ElementNum = m_ProbeElementNum->text().toInt();
	probe.m_Frequency = m_ProbeFrequency->text().toDouble();
	probe.m_FirstConnection = 1;

	//楔块信息
	wedge.m_WedgeID = 544;
	//无楔块
	wedge.m_WedgeAngle = m_WedgeAngle->text().toDouble();
	wedge.m_h1 = m_Wedgeh1->text().toDouble();
	wedge.m_ProbeOffsetX = m_ProbeOffsetX->text().toDouble();
	wedge.m_ProbeOffsetY = m_ProbeOffsetY->text().toDouble();
	wedge.m_h2 = m_Wedgeh2->text().toDouble();
	wedge.m_w1 = m_Wedgew1->text().toDouble();
	wedge.m_Velocity = m_WedgeVelocity->text().toDouble();

	//扫查信息
	ScanInfo scan;
	scan.m_Type = (ScanType)m_ScanType->currentIndex();
	scan.m_VelocityType = (VelocityType)m_VelocityType->currentIndex();
	scan.m_Reflect = m_Reflect->text().toInt();
	scan.m_ChannelSize = m_ChannelSize->text().toInt();
	scan.m_AngleStart = m_AngleStart->text().toDouble();
	scan.m_AngleStop = m_AngleStop->text().toDouble();
	scan.m_ElementStart = m_ElementStart->text().toInt();
	scan.m_ElementStop = m_ElementStop->text().toInt();
	scan.m_SendFocus = m_SendFocus->text().toDouble();
	scan.m_RecvFocusStart =m_RecvFocusStart->text().toDouble();
	scan.m_RecvFocusEnd = m_RecvFocusEnd->text().toDouble();
	double recStep = m_RecvFocusStep->text().toDouble();
	scan.m_RecvFocusNum = (int)((scan.m_RecvFocusEnd - scan.m_RecvFocusStart) / recStep) + 1;
	scan.m_FocalType = m_FocalType->currentIndex() + 1;

	if (m_ScanType->currentIndex() == 0)
	{
		double step = m_AngleStep->text().toDouble();
		scan.m_ChannelNum = (int)((scan.m_AngleStop - scan.m_AngleStart) / step) + 1;
	}
	else if (m_ScanType->currentIndex() == 1)
	{
		double step = m_ElementStep->text().toInt();
		scan.m_ChannelNum = (scan.m_ElementStop - scan.m_ElementStart) / step + 1;
	}
	

	PositionInfo position;
	position.m_InspectPos = InspectPos::WeldPDetectPos_UpR;
	position.m_WedgeOffset = 20;

	//creator.setPAInspectInfo(&probe, &wedge, &position, &scan, 1);

	AGain = m_AGain->text().toDouble();
	DGain = m_DGain->text().toDouble();
	pulseWidth = m_PulseWidth->text().toDouble() / 1e3;
	filter.m_FilterType = FilterType::eNoFilter;
	//编码器信息
	encoder.m_ScanAxisID = m_pAxis->currentIndex();
	encoder.m_ScanStart = m_pStart[encoder.m_ScanAxisID]->text().toDouble();
	encoder.m_ScanStep = m_pStep[encoder.m_ScanAxisID]->text().toDouble();
	encoder.m_ScanRange = m_pRange[encoder.m_ScanAxisID]->text().toDouble();
	encoder.m_ScanPointNum = (encoder.m_ScanRange / encoder.m_ScanStep) + 1;
	encoder.m_EncoderResolution[0] = m_pResolution[encoder.m_ScanAxisID]->text().toInt();
	encoder.m_EncoderPolarity[0] = m_pPolarity[encoder.m_ScanAxisID]->currentIndex();

	encoder.m_IndexStart = m_pStart[1 - encoder.m_ScanAxisID]->text().toDouble();
	encoder.m_IndexStep = m_pStep[1 - encoder.m_ScanAxisID]->text().toDouble();
	encoder.m_IndexRange = m_pRange[1 - encoder.m_ScanAxisID]->text().toDouble();
	encoder.m_IndexPointNum = int(encoder.m_IndexRange / encoder.m_IndexStep) + 1;
	encoder.m_EncoderResolution[1] = m_pResolution[1 - encoder.m_ScanAxisID]->text().toInt();
	encoder.m_EncoderPolarity[1] = m_pPolarity[1 - encoder.m_ScanAxisID]->currentIndex();

	/*creator.setPAUTInfo(encoder, filter, AGain, DGain, pulseWidth, divisor);

	//平板对接焊几何信息
	PlateWeldInfo plate;
	//平板对接焊坡口信息
	GrooveInfo groove;
	//材料信息
	creator.getPlateWeldInfo(plate, groove, material);
	material.m_LongitudinalVelocity = m_LongitudinalVelocity->text().toDouble();
	material.m_ShearVelocity = m_ShearVelocity->text().toDouble();
	material.m_Thickness = m_Thickness->text().toDouble();
	creator.setPlateWeldInfo(plate, groove, material);

	creator.createPAInspectSolution(m_pFilePath->text().toLatin1().data());*/

	m_IsConfig = true;

	close();
}

void PAConfigSolutionDlg::modeChanged(int index)
{
	if (index == 0)
	{
		m_ElementStop->setReadOnly(true);
		m_ElementStep->setReadOnly(true);
		m_AngleStop->setReadOnly(false);
		m_AngleStep->setReadOnly(false);
		m_ElementStop->setText(m_ElementStart->text());
		m_ElementStep->setText(QString::number(0, 'f', 0));

		//图片切换
		LinearScan->setVisible(false);
		AngleScan->setVisible(true);		
	}
	else if (index == 1)
	{
		m_AngleStop->setReadOnly(true);
		m_AngleStep->setReadOnly(true);
		m_ElementStop->setReadOnly(false);
		m_ElementStep->setReadOnly(false);
		m_AngleStop->setText(m_AngleStart->text());
		m_AngleStep->setText(QString::number(0, 'f', 0));

		//图片切换
		AngleScan->setVisible(false);
		LinearScan->setVisible(true);
		
	}
}

void PAConfigSolutionDlg::FocalmodeChanged(int i)
{
	//图片切换
	if (m_FocalType->currentIndex() == 0)
	{
		vertical->setVisible(true);
		Horizon->setVisible(false);
		arc->setVisible(false);
	}
	else if (m_FocalType->currentIndex() == 1)
	{
		vertical->setVisible(false);
		Horizon->setVisible(true);
		arc->setVisible(false);
	}
	else
	{
		vertical->setVisible(false);
		Horizon->setVisible(false);
		arc->setVisible(true);
	}
}

void PAConfigSolutionDlg::linear(const QString & string)
{
	if (m_ScanType->currentIndex() == 0)
	{
		m_ElementStop->setText(string);
	}
}

void PAConfigSolutionDlg::angle(const QString & string)
{
	if (m_ScanType->currentIndex() == 1)
	{
		m_AngleStop->setText(string);
	}
}



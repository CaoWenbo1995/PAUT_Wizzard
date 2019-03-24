#pragma once

#include <QDialog>
#include <PADefinition.h>
//#include <PASolutionCreator.h>

class QLineEdit;
class QComboBox;
class QPushButton;
class QTabWidget;
class QLabel;


class PAConfigSolutionDlg :public QDialog
{
	Q_OBJECT

public:
	void setIsConfig(bool config);

	bool isConfig();

	QString getPath();

public:
	//构造函数
	PAConfigSolutionDlg(QWidget* parent = 0);
	
	//析构函数
	~PAConfigSolutionDlg();

private slots:
	//浏览按钮
	void browse();
	//应用按钮
	void apply();
	//检测模式改变
	void modeChanged(int i);
	//检测模式改变
	void FocalmodeChanged(int i);
	//linear模式设定
	void linear(const QString & string);
	//angle模式设定
	void angle(const QString & string);

private:
	//设置界面所需信息
	//探头信息
	ProbeInfo probe;
	//楔块信息
	WedgeInfo wedge;
	//扫查信息
	ScanInfo scan;
	//编码器信息
	EncoderInfo encoder;
	//滤波器
	FilterInfo filter;

	//超声信息
	//模拟增益
	double AGain;
	//数字增益
	double DGain;
	//分频数
	int divisor;
	//脉冲宽度
	double pulseWidth;

	MaterialInfo material;
	
	//所需控件信息
	//探头参数界面
	//探头ID
	QLineEdit* m_ProbeID;
	//探头名称
	QLineEdit* m_ProbeName;
	//探头高度
	QLineEdit* m_ProbeHeight;
	//阵元间距
	QLineEdit* m_ProbePitch;
	//阵元数量
	QLineEdit* m_ProbeElementNum;
	//探头频率
	QLineEdit* m_ProbeFrequency;

	//扫描参数界面
	//电压
	QLineEdit* m_AGain;
	//超声频率
	QLineEdit* m_DGain;
	//采样分频数
	QLineEdit* m_Divisor;
	//脉冲宽度
	QLineEdit* m_PulseWidth;

	//楔块界面
	QLineEdit* m_WedgeAngle;
	QLineEdit* m_Wedgeh1;
	QLineEdit* m_ProbeOffsetX;
	QLineEdit* m_Wedgeh2;
	QLineEdit* m_Wedgew1;
	QLineEdit* m_ProbeOffsetY;
	QLineEdit* m_WedgeVelocity;
	QLineEdit* m_WedgeOffset;

	//扫查类型
	QComboBox* m_ScanType;
	QComboBox* m_VelocityType;
	QLineEdit* m_Reflect;
	QLineEdit* m_ChannelSize;
	QLineEdit* m_AngleStart;
	QLineEdit* m_AngleStop;
	QLineEdit* m_AngleStep;
	QLineEdit* m_ElementStart;
	QLineEdit* m_ElementStop;
	QLineEdit* m_ElementStep;
	QLineEdit* m_SendFocus;
	QLineEdit* m_RecvFocusStart;
	QLineEdit* m_RecvFocusEnd;
	QLineEdit* m_RecvFocusStep;
	QComboBox* m_FocalType;

	//工件材料界面
	//材料名称
	QLineEdit* m_MaterialName;
	//材料纵波声速
	QLineEdit* m_ShearVelocity;
	//材料横波声速
	QLineEdit* m_LongitudinalVelocity;
	//材料厚度
	QLineEdit*m_Thickness;

	//编码器界面
	//扫查轴ID 0-x轴，1-y轴
	QComboBox* m_pAxis;
	//扫查轴编码器起点(mm)
	QLineEdit* m_pStart[2];
	//扫查轴编码器范围(mm)
	QLineEdit* m_pRange[2];
	//扫查轴编码器步进(mm)
	QLineEdit* m_pStep[2];
	//扫查轴极性 0 -- 正向 1 -- 反向
	QComboBox* m_pPolarity[2];
	//扫查轴分辨率
	QLineEdit* m_pResolution[2];

	//检测方案存储地址
	QLineEdit* m_pFilePath;

	//浏览界面
	QPushButton* m_pBrowse;
	//应用按钮
	QPushButton* m_pApply;

	//Focus界面图片
	QLabel* AngleScan;
	QLabel* LinearScan;
	QLabel* vertical;
	QLabel* Horizon;
	QLabel* arc;

	//是否进行配置
	bool m_IsConfig;

	//PASolutionCreator creator;

};
/************************************************************************/
/*							  基本数据定义类							*/
/*					    author LiYang   2012 04 21						*/
/************************************************************************/
#pragma once

#include "PAUTPreprocessor.h"

//int型容器
PAUT_VECTOR_POD_DECLARE(PAUTIntVector, int)
//double型容器
PAUT_VECTOR_POD_DECLARE(PAUTDoubleVector, double)
//二维double型容器
PAUT_VECTOR_NOT_POD_DECLARE(PAUTDouble2DVector, PAUTDoubleVector)

enum class PASimOrientation
{
    Right,										//右手系
    Left										//左手系
};

enum class VelocityType
{
    eLongitudinal,								//纵波
    eShear										//横波
};

enum class ScanType
{
    //相控阵
    ePA,
    ePAAngle,
    ePALinear,

	//FMC
	eFMC,

    //水浸脉冲反射
    ePulseEcho,

    //一发一收(TR)
    eTR,

    //eTOFD
    eTOFD,

	//环阵
	eRing,

	//特殊类型
	eSpecial
};

//PA系统工作状态
enum class PASystemStatus
{
	//程序启动但并未开启Pokcet
	eOpen,

	//系统正在加载计算等等时的状态
	eLoading,

	//程序启动且开启了Pocket，但并配置检测方案
	eInit,

	//开启了Pocket并配置了检测方案但是并没有开始校正或者采集
	eConfig,

	//配置完检测方案后进行校正
	eCalibration,

	//开始采集但并未开始扫查
	eAcquire,

	//开始扫查
	eScanning,

	//机器人扫描
	eRobotScanning,

	//开始扫查（时钟触发）
	eScanningTime,

	//暂停扫查(时钟触发)
	eScanningTimePause,

	//扫查暂停
	eScanningPause,

	//分析
	eAnalysis,

	//离线模式
	eOffLine
};

//校准类型
enum class CalibrationType
{
	//无校准
	eNoCalibration,

	//声速校准
	eVelocity,

	//楔块延时校准
	eWedgeDelay,

	//灵敏度校准
	eSensitive,

	//TCG校准
	eTCG
};

//校准试样类型
enum class CalibrationPartType
{
	//半径试样
	eRadius,

	//深度试块
	eDepth,

	//厚度试块
	eThickness
};

//发射接收模式
enum class TRMode
{
	//发射接收(脉冲反射)
	eTransmitReceive,

	//穿透
	eTransmit
};

//表征不同的检波模式
enum class RectificMode
{
	eRF,
	eHWPlus,
	eHWMinus,
	eFW
};

//超声轴类型
enum class UTAxisType
{
	//时间(us)
	eTime,

	//距离
	eDepth,

	//真实距离(mm)
	eTrueDepth
};

//探头类型
enum class ProbeType
{
	//线阵
	eLinear,

	//弧阵
	eArc,

	//环阵
	eRing
};

enum class InspectPos
{
    WeldPDetectPos_Up,
    WeldPDetectPos_Down,
    WeldPDetectPos_UpL,
    WeldPDetectPos_UpR,
    WeldPDetectPos_DownL,
    WeldPDetectPos_DownR,
    WeldFDetectPos_WebUp,
    WeldFDetectPos_WebDown,
    WeldFDetectPos_MomUpL,
    WeldFDetectPos_MomUpR,
    WeldFDetectPos_MomDown
};

enum class PartType
{
    PWeld,										//平板对接焊缝
    FWeld										//角接焊缝
};

enum class GrooveType
{
    GrooveV = 0,						//V型坡口
    GrooveY = 1,						//Y型坡口
    GrooveI = 2,						//I型坡口
    GrooveYY = 3,						//双Y型坡口
    GrooveVV = 4							//双V型坡口
};

//坐标架信息
struct CoordInfo
{
    PASimOrientation		m_Orientation;				//坐标系极性(左手系or右手系)
    double			m_OrignX;					//坐标系原点X坐标
    double			m_OrignY;					//坐标系原点Y坐标
    double			m_Angle;					//坐标系坐标架相对于世界坐标系水平线夹角(0至360度 左手系 和右手系的定义存在着不同)
};

//探头参数信息
struct ProbeInfo
{
	int				m_ProbeID;					//探头ID数据库使用
    char			m_ProbeName[100];			//探头名称
    double			m_Height;					//探头高度
    int				m_ElementNum;				//振源个数
    double			m_Pitch;					//振源间距
    double			m_Frequency;				//探头频率(MHz)
    int				m_FirstConnection;			//探头第一连接处
	double		    m_D;						//探头直径（对于常规和TOFD探头有用）

	//环阵阵元数组
	PAUTDoubleVector m_RingCenterArray;

	//探头半径(该参数仅对弧阵探头有意义)
	double				m_Radius;

	//探头类型 线阵 or 弧阵
	ProbeType		m_ProbeType;
};

//楔块参数信息
struct WedgeInfo
{
	int				m_WedgeID;					//楔块ID数据库使用
	char			m_WedgeName[100];			//楔块名称
    double			m_WedgeAngle;				//楔块倾斜角
    double			m_h1;						//靠近工件原点的竖直边的高度
    double			m_h2;						//远离工件原点的竖直边的高度
    double			m_w1;						//下表面水平线宽度
    double			m_Velocity;					//楔块声速
    double			m_ProbeOffsetX;				//探头相对于楔块后沿的X偏移量
    double			m_ProbeOffsetY;				//探头相对于楔块下表面的Y偏移量
	//其他信息
	PAUTDoubleVector m_OtherInfo;
};

//扫描信息
struct ScanInfo
{
    int				m_ChannelNum;				//通道数
    int				m_ChannelSize;				//通道孔径大小
    ScanType		m_Type;						//扫查类型
    VelocityType	m_VelocityType;				//使用纵波还是横波
    int				m_ElementStart;				//起始振源号
    int				m_ElementStop;				//终止振源号(相控阵角度扫查时该值与起始振源号相同)
    double			m_AngleStart;				//起始偏转角度
    double			m_AngleStop;				//终止偏转角度(相控阵线性扫查时该值与起始角度相同)
    int				m_Reflect;					//反射次数
    double			m_SendFocus;				//发射聚焦深度
    int				m_RecvFocusNum;				//接收动态聚焦次数
    double			m_RecvFocusStart;			//接收动态聚焦起始深度
    double			m_RecvFocusEnd;				//接收动态聚焦终止深度

	//聚焦模式 1 -- 用相同的X坐标算(检测一条竖线)
	//	       2 -- 用相同的Y坐标算(检测一条横线)
	//	       3 -- 用相同的半径算(检测一个圆弧)
	int m_FocalType;
};

//检测位置信息
struct PositionInfo
{
    InspectPos		m_InspectPos;				//检测位置
    double			m_WedgeOffset;				//楔块偏移量，该值的含义与具体的检测方位有关
    double			m_TofdCenterOffset;			//Tofd检测双探头中心相对于对称轴的偏移量(仅对eTOFD检测有用)
    PASimOrientation		m_Orientation;				//楔块坐标系极性(左手or右手)
    double			m_WedgeOffsetX;				//楔块相对于工件的X偏移量
    double			m_WedgeOffsetY;				//楔块相对于工件的Y偏移量
    double			m_Angle;					//楔块坐标系的转角
};

//工件材料信息
struct MaterialInfo
{
	int				m_MaterialID;				//材料的数据库ID号
	char			m_MaterialName[100];		//材料名字
    PartType		m_Type;						//工件类型(平板对接焊 或者 角接焊)
    double			m_ShearVelocity;			//纵波速度
    double			m_LongitudinalVelocity;		//横波速度
    double			m_Thickness;				//材料厚度
};

//焊缝主体几何信息
struct PlateWeldInfo
{
    double			m_MotherWidth;				//母材宽度
    double			m_MotherThickness;			//母材高度
    double			m_UpWeldWidth;				//上焊缝宽
    double			m_UpWeldHeight;				//上焊缝余高
    double			m_DownWeldWidth;			//下焊缝宽
    double			m_DownWeldHeight;			//下焊缝余高
};

//焊缝坡口信息
struct GrooveInfo
{
    GrooveType		m_GrooveType;				//坡口类型
    double			m_GrooveIb;					//I型坡口b值
    double			m_GrooveYb;					//Y型坡口b值
    double			m_GrooveYP;					//Y型坡口P值
    double			m_GrooveVa;					//V型坡口a值
    double			m_GrooveVb;					//V型坡口b值
    double			m_GrooveYYb;				//双Y型坡口b值
    double			m_GrooveYYP;				//双Y型坡口P值
    double			m_GrooveVVb;				//双V型坡口b值
    double			m_GrooveVVH;				//双V型坡口H值
};

//角焊缝信息
struct FilletWeldInfo
{
    double			m_MotherWidth;				//母材长度
    double			m_MotherThickness;			//母材厚度
    double			m_WebAngle;					//腹板角度
    double			m_WebWidth;					//腹板长度
    double			m_WebThickness;				//腹板厚度
    double			m_BlunteEdgeOffset;			//钝边偏移量
    double			m_BlunteEdgeHeight;			//钝边高度
    double			m_BlunteEdgeWidth;			//钝边长度
    double			m_LeftWeldAngle;			//左焊缝角度
    double			m_RightWeldAngle;			//右焊缝角度
    double			m_LeftWeldWidth;			//左焊缝宽度
    double			m_RightWeldWidth;			//右焊缝宽度
};

//硬件基本设置信息
struct HardwareInfo
{
	//发射电压(V)
	double m_TransmitVoltage;

	//超声频率(Hz)(脉冲重复频率)
	double m_UTFrequency;

	//采样分频数
	int m_SamplingDivisor;

	//扫查频率(Hz)
	double m_ScanningFrequency;
};

//组基本配置信息
struct GroupBasicInfo
{
	//扫查类型
	ScanType m_ScanType;

	//发射接收模式
	TRMode m_TRMode;

	//试件声速
	double m_PartVelocity;

	//采样延迟时间(这个延迟时间和各扫查显示模式的起始相关)
	double m_DigitizingDelay;

	//楔块采样延迟时间(这个延迟时间不会导致各扫查显示模式的起始发生变化)
	double m_WedgeDitizingDelay;

	//通道数
	int m_ChannelNum;

	//发射聚焦深度
	double m_SendFocus;

	//接收动态聚焦次数
	int	m_RecvFocusNum;

	//接收动态聚焦起始深度
	double m_RecvFocusStart;

	//接收动态聚焦终止深度
	double m_RecvFocusEnd;

	//聚焦模式 1 -- 用相同的X坐标算(检测一条竖线)
	//	       2 -- 用相同的Y坐标算(检测一条横线)
	//	       3 -- 用相同的半径算(检测一个圆弧)
	int m_FocalType;
};

//FMC采集信息
struct FMCInfo
{
	//阵元数量
	int m_ElementNum;

	//采集分频数
	int m_Divisor;

	//采集频率
	double m_Frequency;

	//采集时长
	double m_AScanDepth;

	//脉冲宽度
	double m_Width;

	//激发模拟增益
	double m_TAGain;

	//激发数字增益
	double m_TDGain;

	//接收模拟增益
	double m_RDGain;

	//存储的最大扫描行数
	int m_NMaxScanLine;
};

//闸门信息
struct GateInfo
{
	enum { eMaxGateNum = 4 };

	//闸门是否使用
	bool m_GateIsUsed;

	//闸门类型 0 -- 直线型 1 -- 圆弧型
	int m_GateType;

	//闸门起始
	double m_GateStart;

	//闸门范围
	double m_GateRange;

	//闸门阈值
	double m_GateThreshold;
};

//声束基本信息
struct BeamBasicInfo
{
	double m_WedgeDelay;
	double m_BeamAngle;
	int m_CenterElementID;
};

//编码器信息
struct EncoderInfo
{
	//扫查轴编码器起点(mm)
	double m_ScanStart;

	//扫查轴编码器范围(mm)
	double m_ScanRange;

	//扫查轴编码器步进(mm)
	double m_ScanStep;

	//扫查轴点数
	int m_ScanPointNum;

	//步进轴编码器起点(mm)
	double m_IndexStart;

	//步进轴编码器范围(mm)
	double m_IndexRange;

	//步进轴编码器步进(mm)
	double m_IndexStep;

	//步进轴点数
	int m_IndexPointNum;

	//扫查轴ID(两个编码器哪一个是扫描轴 哪一个是步进轴)
	//0 -- 编码器1是扫描轴
	//1 -- 编码器2是扫描轴
	int m_ScanAxisID;

	//编码器器极性 0 -- 正向 1 -- 反向
	int m_EncoderPolarity[2];

	//编码器分辨率
	int m_EncoderResolution[2];
};

//存储了某shot的某个逻辑阵元晶片参数信息的结构体
struct ElementInfo
{
	//阵元的脉冲宽度(仅对发射阵元有意义)(ns)
	double m_PulseWidth;

	//阵元的增益(dB)
	double m_Gain;
};
//ElementInfo型容器
PAUT_VECTOR_NOT_POD_DECLARE(PAUTElementInfoVector, ElementInfo)

//存储了一组聚焦法则所包含的信息的结构体
struct FocalLawInfo
{
	//聚焦法则采样深度(us)数组
	double m_AScanDepth;

	//该组聚焦法则所使用的各个阵元的阵元ID数组
	PAUTIntVector m_ElementID;

	//该组聚焦法则所使用的各个阵元的延迟时间数组
	PAUTDoubleVector m_DelayTime;

	//该组聚焦法则所使用的各个阵元的阵元信息数组(对发射阵元有意义)
	PAUTElementInfoVector m_ElementInfo;
};
//FocalLawInfo型容器
PAUT_VECTOR_NOT_POD_DECLARE(PAUTFocalLawInfoVector, FocalLawInfo)

//硬件闸门信息
struct HardwareGateInfo
{
	//是否使用
	bool m_bEnable;

	//闸门起始(us)
	double m_GateStart;

	//闸门范围(us)
	double m_GateRange;

	//闸门阈值(%)
	double m_GateThreshold;

	//闸门峰值模式 0 -- 绝对值最大 1 -- 最大正值 2 -- 最大负值 3 -- 最大正值与最大负值之差
	int m_GateAmpMode;

	//闸门TOF模式 0 -- 峰值对应的时间 1 -- 第一次穿过闸门的时间 2 -- 穿过闸门后第一次经过0的时间 3 -- 穿过闸门后最后一次经过0的时间
	int m_GateTOFMode;

	//闸门跟踪模式 0 -- 无跟踪 1 -- 跟踪闸门起始 2 -- 跟踪闸门终止
	int m_GateTrackMode;

	//被跟踪的闸门ID
	int m_TrackGateID;
};
//HardwareGateInfo型容器
PAUT_VECTOR_NOT_POD_DECLARE(PAUTHardwareGateInfoVector, HardwareGateInfo)

//存储了某个组的某个检测通道的设置参数信息的结构体
struct ChannelInfo
{
	//声束基本信息
	BeamBasicInfo m_BaseInfo;

	//发射聚焦法则
	FocalLawInfo m_TransmitFocalLaw;

	//接收聚焦法则数组
	PAUTFocalLawInfoVector m_ReceiveFocalLaw;

	//硬件闸门信息
	PAUTHardwareGateInfoVector m_HardwareGateInfo;
};

//存储了某个组的校准信息的结构体
struct CalibrationInfo
{
	//楔块延时补偿值数组
	PAUTDoubleVector m_WedgeDelay;

	//灵敏度补偿值数组
	PAUTDoubleVector m_Gain;

	//DAC补偿点时间(每个通道都有一条DAC曲线)
	PAUTDouble2DVector m_DACTime;

	//DAC补偿点的增益(每个通道都有一条DAC曲线)
	PAUTDouble2DVector m_DACGain;

	//是否完成声速校准
	bool m_IsVeclocityCali;

	//是否完成楔块延时校准
	bool m_IsWedgeDelayCali;

	//是否完成灵敏度校准
	bool m_IsSensitiveCali;

	//是否完成TCG校准
	bool m_IsTCGCali;
};

//滤波器类型枚举类
enum class FilterType
{
	//无滤波器
	eNoFilter,

	//低通滤波器
	eLowPassFilter,

	//带通滤波器
	eBandPassFilter,

	//包络滤波器
	eEnvellopFilter
};

struct FilterInfo
{
	//滤波器类型
	FilterType m_FilterType;

	//频率1
	double m_Frequency1;

	//频率2
	double m_Frequency2;
};


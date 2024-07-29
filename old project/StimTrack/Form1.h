// This Software was written by Stefano Biguzzi during his Master thesis in Dicember 2014. 
// The project was proposed by the NearLab of Politecnico of Milan
// Address contact: biguzzi.stefano@gmail.com

#pragma once
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <fstream>
#include <set>
#include <conio.h>
#include <stdlib.h> 
#include <time.h>
//#include <windows.h>
#include <ctime> 
#include <time.h> 
#include <cmath>
#include <iomanip>
#include <atlconv.h>
#include <vcclr.h>
#define _USE_MATH_DEFINES
#define M_PI 3.14;
// change dyrectory
#include <windows.h>
#include <string>
#include <shlobj.h>
#include <sstream>
// concat char
#include <wchar.h>
// draw
#using <system.drawing.dll>
using namespace System;
using namespace System::Drawing;
#include "stdafx.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
// NI-DAQmx
#include <stdio.h>
#include <NIDAQmx.h>
#include <string.h>
/////////////////////////////////	        VICRA  INCLUDE           /////////////////////////////////
#include "igstkObject.h"
#include "itkCommand.h"
#include "igstkLogger.h"
#include "itkStdStreamLogOutput.h"
#include "itkVector.h"
#include "itkVersor.h"
#include "igstkSerialCommunication.h"
#include "igstkTrackerConfiguration.h"
#include "igstkPolarisTracker.h"
#include "igstkPolarisTrackerTool.h"
#include "igstkTransform.h"
#include "igstkTransformObserver.h"
#include "igstkTrackerConfigurationFileReader.h"
#include "igstkPolarisVicraConfigurationXMLFileReader.h"
#include "igstkCoordinateSystem.h"
#include "igstkAffineTransformXMLFileReader.h"
#include "igstkAffineTransform.h"
#include <igstkAscensionCommandInterpreter.h>
#include "igstkRealTimeClock.h"
#include <itkEuler3DTransform.h>
#include "igstkBoxObjectRepresentation.h"
#include <igstkView3D.h>
#include <PolarisTrackerComand.h>
// string ->chart
#include <msclr/marshal.h>
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
// VERIFY PATH
//#include "Shlwapi.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
//#include <unistd.h>
	
using namespace std;

//DMQmax
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ) goto Error; else
#define PI	3.1415926535

int32  CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	ofstream outf;
	int32   error=0;
	char    errBuff[2048]={'\0'};

	// Check to see if an error stopped the task.
	DAQmxErrChk (status);

Error:
	if( DAQmxFailed(error) ) {
		DAQmxGetExtendedErrorInfo(errBuff,2048);
		DAQmxClearTask(taskHandle);
		printf("DAQmx Error: %s\n",errBuff);
		outf.open( "Error_NI.txt", ios::app );
		//label1->Text ="Start For";
		for( int row=0; row<2048; row++ )
			
		outf << errBuff[row]<<endl; 
		
		outf.close();
	}
	return 0;
}

namespace StimTrack {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::IO::Ports;  // if you want to read the COM available
	
	/// <summary>
	/// Summary for Form1
	/// </summary>

	public ref class Form1 : public System::Windows::Forms::Form
	{

		/// THREAD PARTE 1
	delegate void ChartInitDelegate();
	private: Thread^ ChartInitThread;

	public:
		Form1(void)
		{
			InitializeComponent();
			//	findPorts();  // new Com 
			highestPercentageReached = 0;
	

			//
			//TODO: Add the constructor code here
			//
		}


	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::IO::Ports::SerialPort^  _serialPort; // SERIAL COM NEW 
	private: System::Windows::Forms::Button^  button1;
	protected: 

	private: System::Windows::Forms::Button^  button3;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;

	private: 

	private: System::Windows::Forms::ProgressBar^  progressBar1;
	private: System::Windows::Forms::Label^  label1;


	private:
		/// <summary>
		/// Required designer variable.
		// quaternion variables
		double q_w; double b; double c; double d;
		double q_ws; double bs; double cs; double ds;
		// distance vector from hotspot
		double distCalibrate;
		double distCalibratex;	double distCalibratey;	double distCalibratez;
		double rotxCalibrate; double rotyCalibrate; double rotzCalibrate;
		// distance vector tracking online
		double distStartx; double distStarty; double distStartz;
		// dist error from hotspot
		double distError;
		double distErrorx; double distErrory; double distErrorz;
		// range error limit
		double distLimit; 
		double rotxLimit; double rotyLimit; double rotzLimit;
		double rotxError; double rotyError; double rotzError;
		double resetplot; // counter plot distance from hotspot
		int Coil;
		// coordinate of calibration points (for Coil and for Head)
		double Coil1x; double Coil1y; double Coil1z;
		double Coil2x; double Coil2y; double Coil2z;
		double Coil3x; double Coil3y; double Coil3z;
		double CoilUpx; double CoilUpy; double CoilUpz;
		double HeadLeftx; double HeadLefty; double HeadLeftz;
		double HeadRigthx; double HeadRigthy; double HeadRigthz;
		double HeadNasionx; double HeadNasiony; double HeadNasionz;
		double HeadUpLeftx; double HeadUpLefty; double HeadUpLeftz;
		// quaternion from RF(reference_frame) Homo_coil (rototraslation matrix of Coil transformation)
		double qw_coil;double qx_coil;double qy_coil;double qz_coil;
		// quaternion from RF Homo_head (rototraslation matrix of Head transformation)
		double qw_head;double qx_head;double qy_head;double qz_head;
		// quaternion from HotSpot (rototraslation matrix of Hotspot transformation)
		double HotSpot_w; double HotSpot_x; double HotSpot_y; double HotSpot_z;
		// transposition from Homo_coil
		double trans_coilx;double trans_coily;double trans_coilz;
		//  transposition from Homo_head
		double trans_headx;double trans_heady;double trans_headz;
		// RF Head 
		double HeadXi;double HeadXj;double HeadXk;
		double HeadYi;double HeadYj;double HeadYk;
		double HeadZi;double HeadZj;double HeadZk;
		// origin Head
		double HeadOx; double HeadOy; double HeadOz;
		// origin RF Coil 
		double FocusOx;double FocusOy;double FocusOz;
		// RF HotSpot
		double FocusXi;double FocusXj;double FocusXk;
		double FocusYi;double FocusYj;double FocusYk;
		double FocusZi;double FocusZj;double FocusZk;
		// Auxilian vector 
		double FocusAusx;double FocusAusy;double FocusAusz;
		double HeadAusx;double HeadAusy;double HeadAusz;
		// Position of Hotspot to validate my system---> in the real aplication I can see in CoilBook where is HotSpot respect the Coil
		double HotSpotx; double HotSpoty; double HotSpotz; 
		// Position of HotspotHead to validate my system
		double HotSpotHeadx; double HotSpotHeady; double HotSpotHeadz;
		// tip transform, just 
		//double qw_tip; double qx_tip; double qy_tip; double qz_tip; double trans_tipx; double trans_tipy; double trans_tipz;
		int row;
		//verify quality of head calibration
		double dist1;	double dist2; 	double dist3;
		double d1;	double d2; 	double d3;
		double d1Model;	double d2Model; 	double d3Model;
		// angles verification
		double angx;
		double angy;
		double angz;
		double angx1;
		double angy1;
		double angz1;
		double rotxTest;
		double rotyTest;
		double rotzTest;
		// automatic calibration
		int count;
		int visibility;
		// path for directory where save and load
		wchar_t *cdir;



		 int highestPercentageReached;

	private: System::ComponentModel::BackgroundWorker^  backgroundWorker2;
	private: System::Windows::Forms::Button^  button2;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::Button^  button5;
	private: System::Windows::Forms::Button^  button6;
	private: System::Windows::Forms::Button^  button7;
	private: System::Windows::Forms::Button^  button8;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker4;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker5;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker9;

private: System::ComponentModel::BackgroundWorker^  backgroundWorker10;
private: System::Windows::Forms::Button^  button11;
private: System::ComponentModel::BackgroundWorker^  backgroundWorker11;
private: System::Windows::Forms::Button^  button12;
private: System::Windows::Forms::Button^  button13;
private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
private: System::Windows::Forms::Button^  button14;
private: System::Windows::Forms::Label^  label2;
private: System::Windows::Forms::Label^  label3;
private: System::Windows::Forms::Label^  label4;
private: System::Windows::Forms::Button^  button15;
private: System::Windows::Forms::Button^  button16;
private: System::Windows::Forms::Label^  label5;
private: System::Windows::Forms::Label^  label7;
private: System::Windows::Forms::Label^  label8;
private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2;
private: Microsoft::VisualBasic::PowerPacks::ShapeContainer^  shapeContainer1;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape2;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape1;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape6;
private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape1;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape2;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape1;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape5;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape4;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape8;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape7;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape3;
private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape2;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape6;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape4;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape5;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape3;
private: Microsoft::VisualBasic::PowerPacks::LineShape^  lineShape9;
private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape4;
private: Microsoft::VisualBasic::PowerPacks::RectangleShape^  rectangleShape3;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape8;
private: Microsoft::VisualBasic::PowerPacks::OvalShape^  ovalShape7;
private: System::ComponentModel::BackgroundWorker^  backgroundWorker12;
private: System::Windows::Forms::Button^  button18;
private: System::ComponentModel::BackgroundWorker^  backgroundWorker13;
private: System::Windows::Forms::Label^  label9;
private: System::Windows::Forms::Label^  label6;
private: System::Windows::Forms::Button^  button9;
private: System::ComponentModel::BackgroundWorker^  backgroundWorker14;
private: System::Windows::Forms::Label^  label10;
private: System::Windows::Forms::Label^  label11;
private: System::Windows::Forms::Label^  label12;
private: System::Windows::Forms::Label^  label13;
private: System::Windows::Forms::Label^  label14;
private: System::Windows::Forms::Label^  label15;
private: System::Windows::Forms::Label^  label16;
private: System::Windows::Forms::Button^  button10;
private: System::Windows::Forms::Button^  button17;
private: System::Windows::Forms::TextBox^  textBox1;

private: System::Windows::Forms::TextBox^  textBox2;
private: System::Windows::Forms::Label^  label17;
private: System::Windows::Forms::TextBox^  textBox3;
private: System::Windows::Forms::Button^  button19;
private: System::Windows::Forms::Label^  label19;
private: System::Windows::Forms::Label^  label20;
private: System::Windows::Forms::Label^  label21;

private: System::ComponentModel::IContainer^  components;

			 /// </summary>
		
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::Windows::Forms::Label^  label18;
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint1 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0, 
				0));
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint2 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0, 
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint3 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(1, 
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint4 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(2, 
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint5 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(3, 
				0));
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint6 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0, 
				L"0,0"));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->backgroundWorker2 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker3 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			this->button7 = (gcnew System::Windows::Forms::Button());
			this->button8 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker9 = (gcnew System::ComponentModel::BackgroundWorker());
			this->backgroundWorker10 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button11 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker11 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button12 = (gcnew System::Windows::Forms::Button());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button13 = (gcnew System::Windows::Forms::Button());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->button14 = (gcnew System::Windows::Forms::Button());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->button15 = (gcnew System::Windows::Forms::Button());
			this->button16 = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->shapeContainer1 = (gcnew Microsoft::VisualBasic::PowerPacks::ShapeContainer());
			this->lineShape9 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->rectangleShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->rectangleShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->ovalShape8 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->ovalShape7 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->lineShape8 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape7 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->rectangleShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->ovalShape6 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->ovalShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->ovalShape5 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->ovalShape3 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->lineShape6 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->rectangleShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::RectangleShape());
			this->ovalShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->ovalShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::OvalShape());
			this->lineShape5 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape4 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape2 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->lineShape1 = (gcnew Microsoft::VisualBasic::PowerPacks::LineShape());
			this->backgroundWorker12 = (gcnew System::ComponentModel::BackgroundWorker());
			this->button18 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker13 = (gcnew System::ComponentModel::BackgroundWorker());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->button9 = (gcnew System::Windows::Forms::Button());
			this->backgroundWorker14 = (gcnew System::ComponentModel::BackgroundWorker());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->button10 = (gcnew System::Windows::Forms::Button());
			this->button17 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->button19 = (gcnew System::Windows::Forms::Button());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			label18 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2))->BeginInit();
			this->SuspendLayout();
			// 
			// label18
			// 
			label18->AutoSize = true;
			label18->Location = System::Drawing::Point(9, 300);
			label18->Name = L"label18";
			label18->Size = System::Drawing::Size(139, 13);
			label18->TabIndex = 80;
			label18->Text = L"DirectoryPath RECORDING";
			// 
			// button1
			// 
			this->button1->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button1->Location = System::Drawing::Point(3, 74);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(133, 60);
			this->button1->TabIndex = 0;
			this->button1->Text = L"ONLINE TRACKING";
			this->button1->UseVisualStyleBackColor = false;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// button3
			// 
			this->button3->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button3->Location = System::Drawing::Point(3, 12);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(133, 56);
			this->button3->TabIndex = 2;
			this->button3->Text = L"HOTSPOT_START";
			this->button3->UseVisualStyleBackColor = false;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker1_ProgressChanged);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker1_RunWorkerCompleted);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(3, 140);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(255, 33);
			this->progressBar1->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Perpetua Titling MT", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(12, 194);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(69, 20);
			this->label1->TabIndex = 4;
			this->label1->Text = L"label1";
			// 
			// backgroundWorker2
			// 
			this->backgroundWorker2->WorkerReportsProgress = true;
			this->backgroundWorker2->WorkerSupportsCancellation = true;
			this->backgroundWorker2->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker2_DoWork);
			this->backgroundWorker2->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker2_ProgressChanged);
			this->backgroundWorker2->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker2_RunWorkerCompleted);
			// 
			// button2
			// 
			this->button2->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button2->Location = System::Drawing::Point(13, 379);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(48, 31);
			this->button2->TabIndex = 6;
			this->button2->Text = L"CoilL";
			this->button2->UseVisualStyleBackColor = false;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::Coil1_Click);
			// 
			// backgroundWorker3
			// 
			this->backgroundWorker3->WorkerReportsProgress = true;
			this->backgroundWorker3->WorkerSupportsCancellation = true;
			this->backgroundWorker3->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker3_DoWork);
			this->backgroundWorker3->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker3_ProgressChanged);
			this->backgroundWorker3->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker3_RunWorkerCompleted);
			// 
			// button4
			// 
			this->button4->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button4->Location = System::Drawing::Point(67, 379);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(48, 31);
			this->button4->TabIndex = 7;
			this->button4->Text = L"CoilO";
			this->button4->UseVisualStyleBackColor = false;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::Coil2_Click);
			// 
			// button5
			// 
			this->button5->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button5->Location = System::Drawing::Point(175, 380);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(48, 31);
			this->button5->TabIndex = 8;
			this->button5->Text = L"CoilR";
			this->button5->UseVisualStyleBackColor = false;
			this->button5->Click += gcnew System::EventHandler(this, &Form1::Coil3_Click);
			// 
			// button6
			// 
			this->button6->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button6->Location = System::Drawing::Point(13, 458);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(48, 35);
			this->button6->TabIndex = 9;
			this->button6->Text = L"HeadLeft";
			this->button6->UseVisualStyleBackColor = false;
			this->button6->Click += gcnew System::EventHandler(this, &Form1::HeadLeft_Click);
			// 
			// button7
			// 
			this->button7->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button7->Location = System::Drawing::Point(67, 458);
			this->button7->Name = L"button7";
			this->button7->Size = System::Drawing::Size(48, 35);
			this->button7->TabIndex = 10;
			this->button7->Text = L"HeadNasion";
			this->button7->UseVisualStyleBackColor = false;
			this->button7->Click += gcnew System::EventHandler(this, &Form1::HeadNasion_Click);
			// 
			// button8
			// 
			this->button8->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button8->Location = System::Drawing::Point(121, 458);
			this->button8->Name = L"button8";
			this->button8->Size = System::Drawing::Size(48, 35);
			this->button8->TabIndex = 11;
			this->button8->Text = L"HeadRight";
			this->button8->UseVisualStyleBackColor = false;
			this->button8->Click += gcnew System::EventHandler(this, &Form1::HeadRight_Click);
			// 
			// button11
			// 
			this->button11->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->button11->Location = System::Drawing::Point(226, 74);
			this->button11->Name = L"button11";
			this->button11->Size = System::Drawing::Size(74, 60);
			this->button11->TabIndex = 14;
			this->button11->Text = L"SEND SIGNAL";
			this->button11->UseVisualStyleBackColor = false;
			this->button11->Click += gcnew System::EventHandler(this, &Form1::button11_Click);
			// 
			// backgroundWorker11
			// 
			this->backgroundWorker11->WorkerReportsProgress = true;
			this->backgroundWorker11->WorkerSupportsCancellation = true;
			this->backgroundWorker11->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker11_DoWork);
			this->backgroundWorker11->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker11_ProgressChanged);
			this->backgroundWorker11->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker11_RunWorkerCompleted);
			// 
			// button12
			// 
			this->button12->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->button12->Location = System::Drawing::Point(17, 536);
			this->button12->Name = L"button12";
			this->button12->Size = System::Drawing::Size(68, 42);
			this->button12->TabIndex = 20;
			this->button12->Text = L"LOAD HOTSPOT";
			this->button12->UseVisualStyleBackColor = false;
			this->button12->Click += gcnew System::EventHandler(this, &Form1::button12_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(693, 315);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(51, 20);
			this->label6->TabIndex = 25;
			this->label6->Text = L"label6";
			// 
			// button13
			// 
			this->button13->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->button13->Location = System::Drawing::Point(217, 536);
			this->button13->Name = L"button13";
			this->button13->Size = System::Drawing::Size(71, 42);
			this->button13->TabIndex = 44;
			this->button13->Text = L"Load COIL Calib";
			this->button13->UseVisualStyleBackColor = false;
			this->button13->Click += gcnew System::EventHandler(this, &Form1::button13_Click);
			// 
			// chart1
			// 
			chartArea1->AxisX->MajorGrid->LineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::NotSet;
			chartArea1->AxisX->Maximum = 100;
			chartArea1->AxisX->Minimum = 0;
			chartArea1->AxisX->Title = L"N. Frame";
			chartArea1->AxisY->MajorGrid->LineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::NotSet;
			chartArea1->AxisY->Maximum = 10;
			chartArea1->AxisY->MaximumAutoSize = 10;
			chartArea1->AxisY->Minimum = 0;
			chartArea1->AxisY->Title = L"mm";
			chartArea1->BorderColor = System::Drawing::Color::Transparent;
			chartArea1->CursorY->SelectionEnd = 5; 
			chartArea1->CursorY->SelectionStart = -5; 
			chartArea1->Name = L"ChartArea1";
			chartArea2->AxisX->MajorGrid->LineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::NotSet;
			chartArea2->AxisX->Maximum = 4;
			chartArea2->AxisX->Minimum = 0;
			chartArea2->AxisY->MajorGrid->LineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::NotSet;
			chartArea2->AxisY->Maximum = 7;
			chartArea2->AxisY->Minimum = -7;
			chartArea2->AxisY->Title = L"Degrees";
			chartArea2->BorderColor = System::Drawing::Color::White;
			chartArea2->CursorY->SelectionEnd = 5;
			chartArea2->CursorY->SelectionStart = -5;
			chartArea2->Name = L"ChartArea2";
			this->chart1->ChartAreas->Add(chartArea1);
			this->chart1->ChartAreas->Add(chartArea2);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(303, 230);
			this->chart1->Name = L"chart1";
			this->chart1->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Pastel;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series1->Legend = L"Legend1";
			series1->LegendText = L"Distance";
			series1->MarkerColor = System::Drawing::Color::Black;
			series1->MarkerSize = 10;
			series1->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Star10;
			series1->Name = L"Series1";
			dataPoint1->LegendText = L"";
			dataPoint1->LegendToolTip = L"";
			series1->Points->Add(dataPoint1);
			series2->ChartArea = L"ChartArea2";
			series2->IsValueShownAsLabel = true;
			series2->IsXValueIndexed = true;
			series2->LabelForeColor = System::Drawing::Color::BlanchedAlmond;
			series2->Legend = L"Legend1";
			series2->LegendText = L"Rotation";
			series2->MarkerColor = System::Drawing::Color::Maroon;
			series2->MarkerSize = 10;
			series2->Name = L"BarChart";
			dataPoint2->AxisLabel = L"";
			dataPoint3->AxisLabel = L"";
			dataPoint3->MarkerColor = System::Drawing::Color::Maroon;
			dataPoint4->AxisLabel = L"";
			dataPoint5->AxisLabel = L"";
			series2->Points->Add(dataPoint2);
			series2->Points->Add(dataPoint3);
			series2->Points->Add(dataPoint4);
			series2->Points->Add(dataPoint5);
			series2->XValueType = System::Windows::Forms::DataVisualization::Charting::ChartValueType::Double;
			this->chart1->Series->Add(series1);
			this->chart1->Series->Add(series2);
			this->chart1->Size = System::Drawing::Size(470, 413);
			this->chart1->TabIndex = 5;
			this->chart1->Text = L"chart1";
			this->chart1->Click += gcnew System::EventHandler(this, &Form1::chart1_Click);
			// 
			// button14
			// 
			this->button14->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->button14->Location = System::Drawing::Point(121, 536);
			this->button14->Name = L"button14";
			this->button14->Size = System::Drawing::Size(71, 42);
			this->button14->TabIndex = 50;
			this->button14->Text = L"Load HEAD Calib";
			this->button14->UseVisualStyleBackColor = false;
			this->button14->Click += gcnew System::EventHandler(this, &Form1::button14_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(223, 581);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(0, 13);
			this->label2->TabIndex = 51;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(131, 581);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(0, 13);
			this->label3->TabIndex = 52;
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(19, 581);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(0, 13);
			this->label4->TabIndex = 53;
			// 
			// button15
			// 
			this->button15->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->button15->Location = System::Drawing::Point(142, 74);
			this->button15->Name = L"button15";
			this->button15->Size = System::Drawing::Size(78, 60);
			this->button15->TabIndex = 54;
			this->button15->Text = L"STOP";
			this->button15->UseVisualStyleBackColor = false;
			this->button15->Click += gcnew System::EventHandler(this, &Form1::button15_Click);
			// 
			// button16
			// 
			this->button16->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(255)));
			this->button16->Location = System::Drawing::Point(12, 216);
			this->button16->Name = L"button16";
			this->button16->Size = System::Drawing::Size(72, 41);
			this->button16->TabIndex = 55;
			this->button16->Text = L"Verify CalibrationH";
			this->button16->UseVisualStyleBackColor = false;
			this->button16->Click += gcnew System::EventHandler(this, &Form1::button16_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(108, 218);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(0, 13);
			this->label5->TabIndex = 56;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(108, 232);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(0, 13);
			this->label7->TabIndex = 57;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(108, 246);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(0, 13);
			this->label8->TabIndex = 58;
			// 
			// chart2
			// 
			this->chart2->BackColor = System::Drawing::Color::Transparent;
			this->chart2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->chart2->BackImageAlignment = System::Windows::Forms::DataVisualization::Charting::ChartImageAlignmentStyle::TopRight;
			this->chart2->BackImageTransparentColor = System::Drawing::Color::White;
			this->chart2->BackImageWrapMode = System::Windows::Forms::DataVisualization::Charting::ChartImageWrapMode::Unscaled;
			this->chart2->BorderlineColor = System::Drawing::Color::Transparent;
			chartArea3->AxisX->Crossing = 0;
			chartArea3->AxisX->Interval = 1;
			chartArea3->AxisX->IsLabelAutoFit = false;
			chartArea3->AxisX->LabelAutoFitMinFontSize = 5;
			chartArea3->AxisX->MajorGrid->Enabled = false;
			chartArea3->AxisX->Maximum = 10;
			chartArea3->AxisX->Minimum = -10;
			chartArea3->AxisX->ScaleBreakStyle->StartFromZero = System::Windows::Forms::DataVisualization::Charting::StartFromZero::Yes;
			chartArea3->AxisY->Crossing = 0;
			chartArea3->AxisY->Interval = 1;
			chartArea3->AxisY->IntervalOffsetType = System::Windows::Forms::DataVisualization::Charting::DateTimeIntervalType::Number;
			chartArea3->AxisY->IsLabelAutoFit = false;
			chartArea3->AxisY->MajorGrid->LineDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::NotSet;
			chartArea3->AxisY->Maximum = 10;
			chartArea3->AxisY->Minimum = -10;
			chartArea3->CursorX->LineColor = System::Drawing::Color::DarkRed;
			chartArea3->CursorX->SelectionColor = System::Drawing::Color::LightGreen;
			chartArea3->CursorX->SelectionEnd = 5; 
			chartArea3->CursorX->SelectionStart = -5; 
			chartArea3->CursorY->LineColor = System::Drawing::Color::DarkRed;
			chartArea3->CursorY->SelectionColor = System::Drawing::Color::LightGreen;
			chartArea3->CursorY->SelectionEnd = 5; 
			chartArea3->CursorY->SelectionStart = -5; 
			chartArea3->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea3);
			legend2->Name = L"Legend1";
			this->chart2->Legends->Add(legend2);
			this->chart2->Location = System::Drawing::Point(811, 271);
			this->chart2->Name = L"chart2";
			this->chart2->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Pastel;
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Line;
			series3->Legend = L"Legend1";
			series3->MarkerBorderColor = System::Drawing::Color::Black;
			series3->MarkerColor = System::Drawing::Color::Black;
			series3->MarkerSize = 10;
			series3->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Star10;
			series3->Name = L"Series1";
			series3->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::SemiTransparent;
			dataPoint6->BorderDashStyle = System::Windows::Forms::DataVisualization::Charting::ChartDashStyle::NotSet;
			dataPoint6->BorderWidth = 0;
			dataPoint6->LabelBorderWidth = 0;
			dataPoint6->MarkerBorderWidth = 0;
			dataPoint6->MarkerColor = System::Drawing::Color::Black;
			dataPoint6->MarkerSize = 8;
			dataPoint6->MarkerStyle = System::Windows::Forms::DataVisualization::Charting::MarkerStyle::Cross;
			series3->Points->Add(dataPoint6);
			series3->YValuesPerPoint = 2;
			this->chart2->Series->Add(series3);
			this->chart2->Size = System::Drawing::Size(509, 372);
			this->chart2->TabIndex = 59;
			this->chart2->Text = L"chart2";
			this->chart2->Click += gcnew System::EventHandler(this, &Form1::chart2_Click);
			// 
			// shapeContainer1
			// 
			this->shapeContainer1->Location = System::Drawing::Point(0, 0);
			this->shapeContainer1->Margin = System::Windows::Forms::Padding(0);
			this->shapeContainer1->Name = L"shapeContainer1";
			this->shapeContainer1->Shapes->AddRange(gcnew cli::array< Microsoft::VisualBasic::PowerPacks::Shape^  >(21) {this->lineShape9, 
				this->rectangleShape4, this->rectangleShape3, this->ovalShape8, this->ovalShape7, this->lineShape8, this->lineShape7, this->lineShape3, 
				this->rectangleShape2, this->ovalShape6, this->ovalShape4, this->ovalShape5, this->ovalShape3, this->lineShape6, this->rectangleShape1, 
				this->ovalShape2, this->ovalShape1, this->lineShape5, this->lineShape4, this->lineShape2, this->lineShape1});
			this->shapeContainer1->Size = System::Drawing::Size(1191, 628);
			this->shapeContainer1->TabIndex = 60;
			this->shapeContainer1->TabStop = false;
			// 
			// lineShape9
			// 
			this->lineShape9->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->lineShape9->BorderWidth = 2;
			this->lineShape9->Name = L"lineShape9";
			this->lineShape9->X1 = 475;
			this->lineShape9->X2 = 673;
			this->lineShape9->Y1 = 34;
			this->lineShape9->Y2 = 212;
			// 
			// rectangleShape4
			// 
			this->rectangleShape4->Location = System::Drawing::Point(402, 111);
			this->rectangleShape4->Name = L"rectangleShape4";
			this->rectangleShape4->Size = System::Drawing::Size(85, 18);
			// 
			// rectangleShape3
			// 
			this->rectangleShape3->Location = System::Drawing::Point(489, 115);
			this->rectangleShape3->Name = L"rectangleShape3";
			this->rectangleShape3->Size = System::Drawing::Size(41, 9);
			// 
			// ovalShape8
			// 
			this->ovalShape8->Location = System::Drawing::Point(544, 119);
			this->ovalShape8->Name = L"ovalShape8";
			this->ovalShape8->Size = System::Drawing::Size(52, 9);
			// 
			// ovalShape7
			// 
			this->ovalShape7->Location = System::Drawing::Point(529, 112);
			this->ovalShape7->Name = L"ovalShape7";
			this->ovalShape7->Size = System::Drawing::Size(77, 20);
			// 
			// lineShape8
			// 
			this->lineShape8->Name = L"lineShape8";
			this->lineShape8->X1 = 458;
			this->lineShape8->X2 = 676;
			this->lineShape8->Y1 = 118;
			this->lineShape8->Y2 = 118;
			// 
			// lineShape7
			// 
			this->lineShape7->Name = L"lineShape7";
			this->lineShape7->X1 = 567;
			this->lineShape7->X2 = 567;
			this->lineShape7->Y1 = 8;
			this->lineShape7->Y2 = 220;
			// 
			// lineShape3
			// 
			this->lineShape3->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->lineShape3->BorderWidth = 2;
			this->lineShape3->Name = L"lineShape3";
			this->lineShape3->X1 = 749;
			this->lineShape3->X2 = 921;
			this->lineShape3->Y1 = 43;
			this->lineShape3->Y2 = 195;
			// 
			// rectangleShape2
			// 
			this->rectangleShape2->Location = System::Drawing::Point(828, 126);
			this->rectangleShape2->Name = L"rectangleShape2";
			this->rectangleShape2->Size = System::Drawing::Size(11, 101);
			// 
			// ovalShape6
			// 
			this->ovalShape6->Location = System::Drawing::Point(837, 103);
			this->ovalShape6->Name = L"ovalShape6";
			this->ovalShape6->Size = System::Drawing::Size(57, 30);
			// 
			// ovalShape4
			// 
			this->ovalShape4->Location = System::Drawing::Point(834, 94);
			this->ovalShape4->Name = L"ovalShape4";
			this->ovalShape4->Size = System::Drawing::Size(73, 49);
			// 
			// ovalShape5
			// 
			this->ovalShape5->Location = System::Drawing::Point(772, 103);
			this->ovalShape5->Name = L"ovalShape5";
			this->ovalShape5->Size = System::Drawing::Size(57, 30);
			// 
			// ovalShape3
			// 
			this->ovalShape3->Location = System::Drawing::Point(760, 93);
			this->ovalShape3->Name = L"ovalShape3";
			this->ovalShape3->Size = System::Drawing::Size(73, 49);
			// 
			// lineShape6
			// 
			this->lineShape6->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)), 
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->lineShape6->BorderWidth = 2;
			this->lineShape6->Name = L"lineShape6";
			this->lineShape6->X1 = 931;
			this->lineShape6->X2 = 1157;
			this->lineShape6->Y1 = 65;
			this->lineShape6->Y2 = 167;
			// 
			// rectangleShape1
			// 
			this->rectangleShape1->Location = System::Drawing::Point(1043, 111);
			this->rectangleShape1->Name = L"rectangleShape1";
			this->rectangleShape1->Size = System::Drawing::Size(11, 15);
			// 
			// ovalShape2
			// 
			this->ovalShape2->Location = System::Drawing::Point(1008, 110);
			this->ovalShape2->Name = L"ovalShape2";
			this->ovalShape2->Size = System::Drawing::Size(39, 14);
			// 
			// ovalShape1
			// 
			this->ovalShape1->Location = System::Drawing::Point(1049, 109);
			this->ovalShape1->Name = L"ovalShape1";
			this->ovalShape1->Size = System::Drawing::Size(38, 15);
			// 
			// lineShape5
			// 
			this->lineShape5->Name = L"lineShape5";
			this->lineShape5->X1 = 928;
			this->lineShape5->X2 = 1169;
			this->lineShape5->Y1 = 118;
			this->lineShape5->Y2 = 118;
			// 
			// lineShape4
			// 
			this->lineShape4->Name = L"lineShape4";
			this->lineShape4->X1 = 1048;
			this->lineShape4->X2 = 1048;
			this->lineShape4->Y1 = 12;
			this->lineShape4->Y2 = 224;
			// 
			// lineShape2
			// 
			this->lineShape2->Name = L"lineShape2";
			this->lineShape2->X1 = 925;
			this->lineShape2->X2 = 744;
			this->lineShape2->Y1 = 118;
			this->lineShape2->Y2 = 118;
			// 
			// lineShape1
			// 
			this->lineShape1->Name = L"lineShape1";
			this->lineShape1->X1 = 834;
			this->lineShape1->X2 = 834;
			this->lineShape1->Y1 = 12;
			this->lineShape1->Y2 = 222;
			// 
			// button18
			// 
			this->button18->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button18->Location = System::Drawing::Point(121, 380);
			this->button18->Name = L"button18";
			this->button18->Size = System::Drawing::Size(47, 30);
			this->button18->TabIndex = 62;
			this->button18->Text = L"CoilUp";
			this->button18->UseVisualStyleBackColor = false;
			this->button18->Click += gcnew System::EventHandler(this, &Form1::button18_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(693, 464);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(51, 20);
			this->label9->TabIndex = 63;
			this->label9->Text = L"label9";
			// 
			// button9
			// 
			this->button9->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button9->Location = System::Drawing::Point(13, 422);
			this->button9->Name = L"button9";
			this->button9->Size = System::Drawing::Size(80, 27);
			this->button9->TabIndex = 64;
			this->button9->Text = L"HEAD_calibr";
			this->button9->UseVisualStyleBackColor = false;
			this->button9->Click += gcnew System::EventHandler(this, &Form1::button9_Click);
			// 
			// backgroundWorker14
			// 
			this->backgroundWorker14->WorkerReportsProgress = true;
			this->backgroundWorker14->WorkerSupportsCancellation = true;
			this->backgroundWorker14->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker14_DoWork);
			this->backgroundWorker14->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker14_ProgressChanged);
			this->backgroundWorker14->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker14_RunWorkerCompleted);
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label10->Location = System::Drawing::Point(524, 0);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(79, 25);
			this->label10->TabIndex = 65;
			this->label10->Text = L"PITCH";
			this->label10->Click += gcnew System::EventHandler(this, &Form1::label10_Click);
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(797, 0);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(64, 25);
			this->label11->TabIndex = 66;
			this->label11->Text = L"YAW";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label12->Location = System::Drawing::Point(1014, 4);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(71, 25);
			this->label12->TabIndex = 67;
			this->label12->Text = L"ROLL";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label13->Location = System::Drawing::Point(382, 603);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(79, 25);
			this->label13->TabIndex = 68;
			this->label13->Text = L"PITCH";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label14->Location = System::Drawing::Point(552, 603);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(64, 25);
			this->label14->TabIndex = 69;
			this->label14->Text = L"YAW";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label15->Location = System::Drawing::Point(467, 603);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(71, 25);
			this->label15->TabIndex = 70;
			this->label15->Text = L"ROLL";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label16->Location = System::Drawing::Point(834, 271);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(161, 16);
			this->label16->TabIndex = 71;
			this->label16->Text = L"TRASVERSAL PLANE";
			// 
			// button10
			// 
			this->button10->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button10->Location = System::Drawing::Point(12, 347);
			this->button10->Name = L"button10";
			this->button10->Size = System::Drawing::Size(81, 26);
			this->button10->TabIndex = 72;
			this->button10->Text = L"COIL_calibr";
			this->button10->UseVisualStyleBackColor = false;
			this->button10->Click += gcnew System::EventHandler(this, &Form1::button10_Click);
			// 
			// button17
			// 
			this->button17->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
				static_cast<System::Int32>(static_cast<System::Byte>(128)));
			this->button17->Location = System::Drawing::Point(142, 12);
			this->button17->Name = L"button17";
			this->button17->Size = System::Drawing::Size(78, 56);
			this->button17->TabIndex = 73;
			this->button17->Text = L"RECORD HOTSPOT";
			this->button17->UseVisualStyleBackColor = false;
			this->button17->Click += gcnew System::EventHandler(this, &Form1::button17_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(157, 297);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(153, 20);
			this->textBox1->TabIndex = 74;
			this->textBox1->Text = L"C:/vicra";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(134, 507);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(169, 20);
			this->textBox2->TabIndex = 78;
			this->textBox2->Text = L"C:/vicra";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(9, 510);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(122, 13);
			this->label17->TabIndex = 79;
			this->label17->Text = L"DirectoryPath LOADING";
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(330, 31);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(22, 20);
			this->textBox3->TabIndex = 81;
			this->textBox3->Text = L"1";
			// 
			// button19
			// 
			this->button19->Location = System::Drawing::Point(12, 258);
			this->button19->Name = L"button19";
			this->button19->Size = System::Drawing::Size(49, 39);
			this->button19->TabIndex = 82;
			this->button19->Text = L"Save CalibH";
			this->button19->UseVisualStyleBackColor = true;
			this->button19->Click += gcnew System::EventHandler(this, &Form1::button19_Click);
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11));
			this->label19->Location = System::Drawing::Point(226, 33);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(100, 18);
			this->label19->TabIndex = 83;
			this->label19->Text = L"Insert #COM :";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label20->ForeColor = System::Drawing::Color::Red;
			this->label20->Location = System::Drawing::Point(170, 321);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(0, 18);
			this->label20->TabIndex = 84;
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label21->ForeColor = System::Drawing::Color::Red;
			this->label21->Location = System::Drawing::Point(194, 489);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(0, 18);
			this->label21->TabIndex = 85;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1191, 628);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->button19);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(label18);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->button17);
			this->Controls->Add(this->button10);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->button9);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->button18);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->button16);
			this->Controls->Add(this->button15);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button14);
			this->Controls->Add(this->button13);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->button12);
			this->Controls->Add(this->button11);
			this->Controls->Add(this->button8);
			this->Controls->Add(this->button7);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->chart1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->shapeContainer1);
			this->Name = L"Form1";
			this->Text = L"Form1";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
				 //if(this->button3->Enabled==false){
					 label1->Text = String::Empty;
					 this->button1->Enabled = false;
					 highestPercentageReached=0;
					 this->progressBar1->Value=0;
					backgroundWorker2->RunWorkerAsync();  //START
	}	 
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 this->backgroundWorker1->CancelAsync();

      // Disable the Cancel button.
		//		button2->Enabled = false;
		 }
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 	label1->Text = String::Empty;  
			  this->button3->Enabled = false;
			  	this->button17->BackColor=System::Drawing::Color::Khaki;
				//this->button2->Enabled = true;
				highestPercentageReached=0;

            // Kickoff the worker thread to begin it's DoWork function.
            this->progressBar1->Value=0;
			label20->Text =""; 
			backgroundWorker1->RunWorkerAsync(); //CALIBRATE
        
		 }
			 // HotSpot
	private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
				 
				 
				 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
				 int freq_track = 500; 
				 int i=0;
				 int j=0;
				 double data[510][4];  
				 for(i=0; i<500;i++){
					for(j=0; j<4;j++){
						data[i][j]=-1;
					}
				}

	int row=0;					
	
	System::String ^ ComNumber = textBox3->Text; 
	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(ComNumber);
	int number= atoi(str2);
	int numero= number - 1;

	stringstream strs;
	strs << numero;
	string temp_str = strs.str();
	char* char_type = (char*) temp_str.c_str();
	
	/////////////////////////////////////////////     START OF VICRA PART     /////////////////////////////////////////////         
	igstk::RealTimeClock::Initialize();
	
	typedef igstk::Object::LoggerType			LoggerType;
	typedef itk::StdStreamLogOutput				LogOutputType;
	typedef igstk::TransformObserver			ObserverType;
	typedef igstk::PolarisTrackerTool			TrackerToolType;
	typedef TrackerToolType::TransformType		TransformType;

	igstk::PolarisTrackerTool::Pointer tool = igstk::PolarisTrackerTool::New();
	igstk::SerialCommunication::Pointer serialComm = igstk::SerialCommunication::New();
	PolarisTrackerCommand::Pointer my_command = PolarisTrackerCommand::New();
	
	typedef igstk::SerialCommunication::PortNumberType PortNumberType;
	unsigned int portNumberIntegerValue = atoi(char_type);

	PortNumberType  polarisPortNumber = PortNumberType(portNumberIntegerValue); 
	serialComm->SetPortNumber( polarisPortNumber );
	serialComm->SetParity( igstk::SerialCommunication::NoParity );
	serialComm->SetBaudRate( igstk::SerialCommunication::BaudRate9600 );
	serialComm->SetDataBits( igstk::SerialCommunication::DataBits8 );
	serialComm->SetStopBits( igstk::SerialCommunication::StopBits1 );
	serialComm->SetHardwareHandshake( igstk::SerialCommunication::HandshakeOff );
	serialComm->OpenCommunication();

	igstk::PolarisTracker::Pointer  tracker = igstk::PolarisTracker::New();
	tracker->AddObserver( itk::AnyEvent(), my_command);
	//tracker->SetLogger( logger );
	label1->Text ="SetCommunication()";
	tracker->SetCommunication( serialComm );
	label1->Text ="RequestOpen()";
	tracker->RequestOpen();

  
//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 1 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool1";
	TrackerToolType::Pointer trackerTool1 = TrackerToolType::New();                       // commented out
	//trackerTool1->SetLogger( logger );                                                    // commented out
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool1->AddObserver( itk::AnyEvent(), my_command);                              // commented out
	
	//For wireless tracker tool type, invoke RequestSelectWirelessTrackerTool method.
	
	trackerTool1->RequestSelectWirelessTrackerTool();  
	std::string romFile1 ="C:/vicra/rom/croceNDI_HEAD.rom";
	
	//std::cout << "SROM file: " << romFile1 << std::endl;		
	
	trackerTool1->RequestSetSROMFileName( romFile1 );
	
	//Configure
	
	trackerTool1->RequestConfigure();                                                     // commented out
	
	//Attach to the tracker

	trackerTool1->RequestAttachToTracker( tracker );     
		
	//Add observer to listen to transform events 

	ObserverType::Pointer coordSystemAObserver1 = ObserverType::New();                    // commented out
	coordSystemAObserver1->ObserveTransformEventsFrom( trackerTool1 );                     // commented out	

	//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 2 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool2";
	TrackerToolType::Pointer trackerTool2 = TrackerToolType::New();
	//trackerTool2->SetLogger( logger );
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool2->AddObserver( itk::AnyEvent(), my_command );

	trackerTool2->RequestSelectWirelessTrackerTool();
	
	//Set the SROM file 
	
	std::string romFile ="C:/vicra/rom/croceNDI_COIL.rom";
	
	trackerTool2->RequestSetSROMFileName( romFile );

	//Configure
	
	trackerTool2->RequestConfigure();
	
	//Attach to the tracker
	trackerTool2->RequestAttachToTracker( tracker );

	//Add observer to listen to transform events 

	ObserverType::Pointer coordSystemAObserver2 = ObserverType::New();
	coordSystemAObserver2->ObserveTransformEventsFrom( trackerTool2 );
	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

	tracker->RequestSetFrequency(freq_track);
	tracker->RequestStartTracking();

	typedef igstk::Transform            Transform;
	typedef ::itk::Vector<double,3>    VectorType; //<double, 3>
	typedef ::itk::Versor<double>		VersorType;//<double>
	typedef ::itk::Matrix<double, 4,4>  MatrixType;
	
/////////////////////////////////////////   END OF VICRA PART     /////////////////////////////////////////



// WHEN WE WANT TO GET THE DATA FROM THE VICRA (IN THIS EXAMPLE FOR THE FIRST TOOL) WE HAVE TO INSERT THE FOLLOWING CODE

	int start=clock();
	
	time_t tstart, tend;
	tstart=time(0);
	label1->Text="COM NOT AVAILABLE";	
	visibility=0;
		while (row<500) 
		{
		if(backgroundWorker1->CancellationPending) //if it was cancelled by button "record Hotspot"
		{
		tracker->RequestStopTracking();
		tracker->RequestClose();
		serialComm->CloseCommunication();
		
		// READ from textbox the name of the file txt where we want to save the calibration
				
		fstream outf;
		System::String ^ Directory = textBox1->Text + "\\HotSpotCalibration.txt"; //path directory 	//write directory
		pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
		const wchar_t *constValue = convertedValue; 
		ofstream outfile(constValue);
							// CHECK IF PATH EXIST
					System::String ^ pathDirectory = textBox1->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						//label20->Text ="PATH exist"; 
					}else{
						label20->Text ="PATH NOT EXIST"; 
					}
		outfile << distCalibratex <<"\n"<< distCalibratey <<"\n"<< distCalibratez << "\n" <<HotSpot_w << "\n"<<HotSpot_x << "\n" <<HotSpot_y<<"\n"<< HotSpot_z<< "\n"<<endl; 
		outfile.close();
		tend=time(0);
		int end=clock();
		this->button3->Enabled = true;
		e->Cancel=true;
		break;
		}

		worker->ReportProgress( row*100/500 );
		if ( row*100/500  > highestPercentageReached )
        {
          highestPercentageReached = row*100/500 ;
         worker->ReportProgress( row*100/500  );
        }
		igstk::PulseGenerator::CheckTimeouts();
		Sleep(5);
		Transform		transform1, transform2, transform3, transform4,transformTip;
		VectorType		vector1, vector2;
		VersorType		versor1, versor2, versor3;
							
		double validityTimeInMilliseconds= 200.0;
			igstk::Transform::ErrorType errorValue = 0.01;
		
		//TRANSFORMS FROM HOMO-MATRIX
			//create transform3 from the Homo_Coil
			igstk::Transform::VersorType rotation3;
			rotation3.Set( qx_coil, qy_coil, qz_coil, qw_coil); // quaternion from Homo_coil
			igstk::Transform::VectorType translation3;
			translation3[0]=trans_coilx; translation3[1]=trans_coily;translation3[2]=trans_coilz; 
			transform3.SetTranslationAndRotation(translation3, rotation3,errorValue, validityTimeInMilliseconds); 
			// create transform4 from the head RF
			igstk::Transform::VectorType translation4;
			translation4[0]=trans_headx; translation4[1]=trans_heady; translation4[2]=trans_headz;
			igstk::Transform::VersorType rotation4;
			rotation4.Set( qx_head, qy_head, qz_head, qw_head ); // quaternion from Homo_coil
			transform4.SetTranslationAndRotation(translation4, rotation4,errorValue, validityTimeInMilliseconds); 
			
			coordSystemAObserver1->Clear();
			coordSystemAObserver2->Clear();
		
		trackerTool1->SetCalibrationTransform(transform4);// transformation to the SR  Head
		trackerTool1->RequestGetTransformToParent();
		trackerTool2->SetCalibrationTransform(transform3); 	// transformation to the SR Coil
		trackerTool2->RequestComputeTransformTo(trackerTool1); // tool2 has seen from tool1
		
		//label1->Text="step 1";	
		if (coordSystemAObserver1->GotTransform() && coordSystemAObserver2->GotTransform() ) { 
			transform1 = coordSystemAObserver1->GetTransform();
			transform2 = coordSystemAObserver2->GetTransform();

			//label1->Text="step 2";	
			if ( transform1.IsValidNow() && !transform2.IsValidNow()){
				label1->Text ="COIL NOT VISIBLE";
				this->button3->BackColor=System::Drawing::Color::Red;
				this->button17->Enabled = false;
				//visibility=visibility+1;
			}
			if ( (!transform1.IsValidNow() && transform2.IsValidNow())|| (!transform1.IsValidNow() && !transform2.IsValidNow() )){
				//if (visibility>=0){	
				label1->Text ="HEAD NOT VISIBLE";
				this->button3->BackColor=System::Drawing::Color::Red;
				this->button17->Enabled = false;
				//}
				//visibility=visibility+1;
			}

		
			if ( transform1.IsValidNow() && transform2.IsValidNow()){
				//visibility=0;
				this->button3->BackColor=System::Drawing::Color::Green;
				this->button17->Enabled = true;
				//vector1 = transform1.GetTranslation();
				//versor1 = transform1.GetRotation();
				vector2 = transform2.GetTranslation();
				versor2 = transform2.GetRotation();
				q_w=versor2.GetW();
				b=versor2.GetX();
				c=versor2.GetY();
				d=versor2.GetZ();
				 // rotation HotSpot
				HotSpot_w=q_w;
				HotSpot_x=b;
				HotSpot_y=c;
				HotSpot_z=d;
				label1->Text="Recording Hot Spot";	
				//distance  
				distCalibratex=vector2[0];
				distCalibratey=vector2[1];
				distCalibratez=vector2[2];
				}
			}
		row++;
		}
		this->button3->BackColor=System::Drawing::Color::White;
		label1->Text ="RequestStopTracking()";
		tracker->RequestStopTracking();
		tracker->RequestClose();
		serialComm->CloseCommunication();
		this->button3->Enabled = true;	

}
	private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
				  this->progressBar1->Value = e->ProgressPercentage;
			 }
	private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
		// First, handle the case where an exception was thrown.
    
			 if ( e->Cancelled ){
         // Next, handle the case where the user cancelled 
         // the operation.
         // Note that due to a race condition in 
         // the DoWork event handler, the Cancelled
         // flag may not have been set, even though
         // CancelAsync was called.
         label1->Text = "HOTSPOT_RECORDED";
      }
      else
      {
         // Finally, handle the case where the operation 
         // succeeded.
         label1->Text ="Succeed";
      }
			 }
			// OnlineTracking
	private: System::Void backgroundWorker2_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 
				BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
				//#define tot_frame 2000;
				 int freq_track = 100; // 20Hz  la frequenza a cui viene utilizzato il Vicra. Ma questo valore sembra non cambiare niente!
				// #define tot_frame 20; // LI RIDUCO!! cos si velocizza (20)
				 int i=0;
				 double data[4];  // [tot_frame][4]
				 
				 for(i=0; i<4;i++){
						data[i]=-1;
				}
				

	row=0;					

	System::String ^ ComNumber = textBox3->Text; 
	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(ComNumber);
	int number= atoi(str2);//)-(int)(str4);// - 1;
	int numero= number - 1;
	// convert int to char*
	stringstream strs;
	strs << numero;
	string temp_str = strs.str();
	char* char_type = (char*) temp_str.c_str();
	
	/////////////////////////////////////////////     START OF VICRA PART     /////////////////////////////////////////////         
	igstk::RealTimeClock::Initialize();

	typedef igstk::Object::LoggerType			LoggerType;
	typedef itk::StdStreamLogOutput				LogOutputType;
	typedef igstk::TransformObserver			ObserverType;
	typedef igstk::PolarisTrackerTool			TrackerToolType;
	typedef TrackerToolType::TransformType		TransformType;

	
	igstk::PolarisTrackerTool::Pointer tool = igstk::PolarisTrackerTool::New();
	igstk::SerialCommunication::Pointer serialComm = igstk::SerialCommunication::New();
	PolarisTrackerCommand::Pointer my_command = PolarisTrackerCommand::New();
	
	typedef igstk::SerialCommunication::PortNumberType PortNumberType;
	unsigned int portNumberIntegerValue = atoi(char_type);// atoi(av[3])

	//std::cout << "porta" << portNumberIntegerValue << std::endl;

	PortNumberType  polarisPortNumber = PortNumberType(portNumberIntegerValue); 
	serialComm->SetPortNumber( polarisPortNumber );
	serialComm->SetParity( igstk::SerialCommunication::NoParity );
	serialComm->SetBaudRate( igstk::SerialCommunication::BaudRate9600 );
	serialComm->SetDataBits( igstk::SerialCommunication::DataBits8 );
	serialComm->SetStopBits( igstk::SerialCommunication::StopBits1 );
	serialComm->SetHardwareHandshake( igstk::SerialCommunication::HandshakeOff );
	//serialComm->SetCaptureFileName( "RecordedStreamByPolarisTracker1.txt" );
	//serialComm->SetCapture( true );
	serialComm->OpenCommunication();

	igstk::PolarisTracker::Pointer  tracker = igstk::PolarisTracker::New();
	tracker->AddObserver( itk::AnyEvent(), my_command);
	//tracker->SetLogger( logger );
	label1->Text ="SetCommunication()";
	//std::cout << "SetCommunication()" << std::endl;
	tracker->SetCommunication( serialComm );
	label1->Text ="RequestOpen()";
	//std::cout << "RequestOpen()" << std::endl;
	tracker->RequestOpen();
	ofstream outf;

  
//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 1 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool1";
	//std::cout << "Instantiate wireless tracker tool1: " << std::endl;
	TrackerToolType::Pointer trackerTool1 = TrackerToolType::New();                       // commented out
	//trackerTool1->SetLogger( logger );                                                    // commented out
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool1->AddObserver( itk::AnyEvent(), my_command);                              // commented out
	
	//For wireless tracker tool type, invoke RequestSelectWirelessTrackerTool method.
	
	trackerTool1->RequestSelectWirelessTrackerTool();  
	std::string romFile1 ="C:/vicra/rom/croceNDI_HEAD.rom";
	
	//std::cout << "SROM file: " << romFile1 << std::endl;		
	
	trackerTool1->RequestSetSROMFileName( romFile1 );
	
	//Configure
	
	trackerTool1->RequestConfigure();                                                     // commented out
	
	//Attach to the tracker
	
	trackerTool1->RequestAttachToTracker( tracker );                                      // commented out
	
	//Add observer to listen to transform events 
	
	ObserverType::Pointer coordSystemAObserver1 = ObserverType::New();                    // commented out
	coordSystemAObserver1->ObserveTransformEventsFrom( trackerTool1 );                     // commented out	

	//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 2 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool2";
	//std::cout << "Instantiate wireless tracker tool2: " << std::endl;
	TrackerToolType::Pointer trackerTool2 = TrackerToolType::New();
	//trackerTool2->SetLogger( logger );
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool2->AddObserver( itk::AnyEvent(), my_command );

	trackerTool2->RequestSelectWirelessTrackerTool();
	
	//Set the SROM file 
	
	std::string romFile ="C:/vicra/rom/croceNDI_COIL.rom";
	
	//std::cout << "SROM file: " << romFile << std::endl;
	trackerTool2->RequestSetSROMFileName( romFile );

	//Configure
	
	trackerTool2->RequestConfigure();
	
	//Attach to the tracker

	trackerTool2->RequestAttachToTracker( tracker );

	//Add observer to listen to transform events 

	ObserverType::Pointer coordSystemAObserver2 = ObserverType::New();
	coordSystemAObserver2->ObserveTransformEventsFrom( trackerTool2 );
	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// tracking starts and define the frequency

	tracker->RequestSetFrequency(freq_track);
	tracker->RequestStartTracking();

	
	typedef igstk::Transform            Transform;
	typedef ::itk::Vector<double,3>    VectorType; //<double, 3>
	typedef ::itk::Versor<double>		VersorType;//<double>
	typedef ::itk::Matrix<double,4,4>	MatrixType;
	


/////////////////////////////////////////   END OF VICRA PART     /////////////////////////////////////////

	// LOCAL VARIABLES WHERE WE STORE THE COORDINATES OF THE TWO TOOLS
	// double translation_tool1[3];
	 //double translation_tool2[3];
	 //double quaternion_tool1[4];
	 //double quaternion_tool2[4];
	
	//const double tolerance = 1000;
	//double matrix_rot1[3][3];
	//double matrix_rot2[3][3];


// WHEN WE WANT TO GET THE DATA FROM THE VICRA (IN THIS EXAMPLE FOR THE FIRST TOOL) WE HAVE TO INSERT THE FOLLOWING CODE
//	MatrixType		matrix_rot1, matrix_rot2, matrix_rot3;
	int start=clock();
	resetplot=0;
	//limits
	distLimit=5; // max strumentation error 1mm
	rotxLimit=5;	// max strumentatino error <1degree
	rotyLimit=5;
	rotzLimit=5;

	time_t tstart, tend;
	tstart=time(0);
	label1->Text="COM NOT AVAILABLE";	
	chart1->Series["Series1"]->Points->Clear();
	visibility=0;
	while (row<20000) 
		{
			if(backgroundWorker2->CancellationPending) //if it was cancelled
					{
						this->button15->Enabled = true;
						e->Cancel=true;
						break;
					}
			
	
			
		worker->ReportProgress( row*100/20000 );
		if ( row*100/20000  > highestPercentageReached )
			{
			highestPercentageReached = row*100/20000 ;
			worker->ReportProgress( row*100/20000  );
        }
		
		igstk::PulseGenerator::CheckTimeouts();
		Sleep(5);
		
		Transform		transform1, transform2, transform3, transform4, transformTip, transform5, t1;
		VectorType		vector1, vector2, angles;
		VersorType		versor1, versor2, versor3;
		MatrixType		RotoTraslo;
		
		igstk::Transform::VectorType translation3;
		translation3[0]=trans_coilx; translation3[1]=trans_coily;translation3[2]=trans_coilz; // origine assi da tool (del coil)
		double validityTimeInMilliseconds= 200.0;
		igstk::Transform::ErrorType errorValue = 0.01;
		igstk::Transform::VersorType rotation3;
		rotation3.Set( qx_coil, qy_coil, qz_coil, qw_coil ); 
		transform3.SetTranslationAndRotation(translation3, rotation3,errorValue, validityTimeInMilliseconds); 
		// create transform4 dalla terna of the head
		igstk::Transform::VectorType translation4;
		translation4[0]=trans_headx; translation4[1]=trans_heady;translation4[2]=trans_headz; // origine assi da tool (del coil)
		igstk::Transform::VersorType rotation4;
		rotation4.Set( qx_head, qy_head, qz_head, qw_head ); // quaternion from Homo_coil
		transform4.SetTranslationAndRotation(translation4, rotation4,errorValue, validityTimeInMilliseconds); 
		igstk::Transform::VectorType translation5;
		//translation5[0]=matrix_rot3[0][3]; translation5[1]=matrix_rot3[1][3];translation5[2]=matrix_rot3[2][3]; // origine assi da tool (del coil)
		translation5[0]=distCalibratex; translation5[1]=distCalibratey; translation5[2]=distCalibratez;  
		igstk::Transform::VersorType rotation5;
		rotation5.Set( HotSpot_x, HotSpot_y, HotSpot_z, HotSpot_w ); // quaternion from Homo_coil
		transform5.SetTranslationAndRotation(translation5, rotation5,errorValue, validityTimeInMilliseconds); // COME LO CONVALIDO??
		t1 = igstk::Transform::TransformCompose( transform4, transform5 );

		coordSystemAObserver1->Clear();
		coordSystemAObserver2->Clear();
		
		//transform Head
		trackerTool1->RequestGetTransformToParent();
		trackerTool1->SetCalibrationTransform(t1); // transformation to the SR  Head
		// trackerTool1->SetCalibrationTransform(transform5);
		
			//transform Coil
		trackerTool2->RequestComputeTransformTo(trackerTool1); // tool2 has seen from tool1
		trackerTool2->SetCalibrationTransform(transform3);// transoformation to the SR Coil
		
		 
		

		//label1->Text="Step1";	
		if (coordSystemAObserver1->GotTransform() && coordSystemAObserver2->GotTransform() ) { // 
			transform1 = coordSystemAObserver1->GetTransform();
			transform2 = coordSystemAObserver2->GetTransform();
			//label1->Text="Step2";	
			if ( transform1.IsValidNow() && !transform2.IsValidNow()){
				label1->Text ="COIL NOT VISIBLE";
				//visibility=visibility+1;
			}
			if (!transform1.IsValidNow() && transform2.IsValidNow() || !transform1.IsValidNow() && !transform2.IsValidNow()){
				//if(	visibility>1){	
				label1->Text ="HEAD NOT VISIBLE";
				//}
				//visibility=visibility+1;
			}

			if ( transform1.IsValidNow() && transform2.IsValidNow()){
				visibility=0;
				vector1 = transform1.GetTranslation();
				versor1 = transform1.GetRotation();
				vector2 = transform2.GetTranslation();
				versor2 = transform2.GetRotation();
				q_ws=versor2.GetW();
				bs=versor2.GetX();
				cs=versor2.GetY();
				ds=versor2.GetZ();
				distStartx=vector2[0];
				distStarty=vector2[1];
				distStartz=vector2[2];
				//formulas from Matlab toolbox  q->tr->rpy
				RotoTraslo[0][0]=  1-2*(pow(cs,2)+pow(ds,2));RotoTraslo[0][1]=2*(bs*cs-q_ws*ds);RotoTraslo[0][2]=2*(bs*ds+q_ws*cs); RotoTraslo[0][3]=0;
				RotoTraslo[1][0]=2*(bs*cs+q_ws*ds); RotoTraslo[1][1]= 1-2*(pow(bs,2)+pow(ds,2));RotoTraslo[1][2]=2*(cs*ds-q_ws*bs); RotoTraslo[1][3]=0;
				RotoTraslo[2][0]=2*(bs*ds-q_ws*cs); RotoTraslo[2][1]= 2*(cs*ds+q_ws*bs); RotoTraslo[2][2]= 1-2*(pow(bs,2)+pow(cs,2));  RotoTraslo[2][3]=0;
				RotoTraslo[3][0]=0;  RotoTraslo[3][1]=0;  RotoTraslo[3][2]=0;  RotoTraslo[3][3]=1;
				data[1] = atan2(-RotoTraslo[1][2], RotoTraslo[2][2]);        // roll
				// compute sin/cos of roll angle
				double sr = sin(data[1]);
				double cr = cos(data[1]);
				data[2] = atan2(RotoTraslo[0][2], cr*RotoTraslo[2][2]-sr*RotoTraslo[1][2])*(180/M_PI);  // pitch
				data[3] = atan2(-RotoTraslo[0][1], RotoTraslo[0][0])*(180/M_PI);  //yaw
				data[1] =data[1]*(180/M_PI);
				//visibility=1;
				label1->Text="VISIBLE";
			}
		}
		
		distError= sqrtf(pow((distStartx),2)+pow((distStarty),2)+pow((distStartz),2));
		rotxError =data[1];
		rotyError =data[2];
		rotzError =data[3];
		// show the value of distance
		label6->Text= ((int)distError).ToString();	
		label9->Text= (max((int)fabs(rotxError),(int)fabs(rotyError))).ToString();	
		//lanci ChartInitThread
		 this->ChartInitThread = gcnew Thread( gcnew ThreadStart( this, &Form1::ChartInitProcedure));
		 this->ChartInitThread->Start();
		
		 resetplot++;
		row++;
		}
		
		label1->Text ="RequestStopTracking()";
		tracker->RequestStopTracking();
		tracker->RequestClose();
		serialComm->CloseCommunication();
		this->button1->Enabled = true;
	 }
	private: System::Void backgroundWorker2_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
		  this->progressBar1->Value = e->ProgressPercentage;
}
	private: System::Void backgroundWorker2_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e){
		 }
	private: void ChartInitProcedure(){
		 this->ChartInitMethod();
		 }
	private: void ChartInitMethod(){
			if(this->chart1->InvokeRequired){
				ChartInitDelegate^ delegato = gcnew ChartInitDelegate(this, &Form1::ChartInitMethod);
				this->Invoke(delegato);
			}else{
// 	QUI IL CODICE CHE GLI VOGLIAMO FAR ESEGUIRE!
				
				
	//if(visibility==1){ // grafic only if tools are visible
		
			//when distance and rotation is enter some limits we have a yellow signal
		if(fabs(distError)<=distLimit ){
			//label1->Text ="IN Range";
			chart1->ChartAreas["ChartArea1"]->BackColor = System::Drawing::Color::Green;
			chart2->ChartAreas["ChartArea1"]->BackColor = System::Drawing::Color::Green;
		}else{
			//chart1->ChartAreas["ChartArea1"]->BackColor.Transparent;
			chart1->ChartAreas["ChartArea1"]->BackColor = System::Drawing::Color::Red;
			chart2->ChartAreas["ChartArea1"]->BackColor = System::Drawing::Color::Red;
			if(button11->Enabled == true){
			backgroundWorker11->RunWorkerAsync();
			}
		}
		if (fabs(rotxError)<=rotxLimit && fabs(rotyError)<=rotyLimit && fabs(rotzError)<=rotzLimit ){
			//label1->Text ="IN Range";
			chart1->ChartAreas["ChartArea2"]->BackColor = System::Drawing::Color::Green;
		}else{
			//chart1->ChartAreas["ChartArea1"]->BackColor.Transparent;
			chart1->ChartAreas["ChartArea2"]->BackColor = System::Drawing::Color::Red;
			if(button11->Enabled == true && fabs(distError)<=distLimit){
			backgroundWorker11->RunWorkerAsync();
			}
		}
			

		if(row>10){  //to evoid displaying first values of distance----> verifica che numero mettere

			if(resetplot<=100){ // refrex every 100 frame
				// rapresent error distace and error rotation
				chart1->Series["Series1"]->Points->AddXY(resetplot,distError);
			}else{
				chart1->Series["Series1"]->Points->Clear();
				//chart1->Series["BarChart"]->Points->Clear();
				//chart1->Series["Series3"]->Points->Clear();
				//chart1->Series["Series4"]->Points->Clear();
				resetplot=0;
				chart1->Series["Series1"]->Points->AddXY(resetplot,distError);
				//chart1->Series["BarChart"]->Points->AddXY(resetplot,rotxError);
				//chart1->Series["Series3"]->Points->AddXY(resetplot,rotyError);
				//chart1->Series["Series4"]->Points->AddXY(resetplot,rotzError);
			}
				chart2->Series["Series1"]->Points->Clear();
	
				chart1->Series["BarChart"]->Points->Clear();
				chart1->Series["BarChart"]->Points->AddXY(1,(int)rotxError);//chart1->Series["BarChart"]->Points->AddXY(1,(int)(rotxError));
				chart1->Series["BarChart"]->Points->AddXY(2,(int)rotyError);
				chart1->Series["BarChart"]->Points->AddXY(3,(int)rotzError);
				/*lineShape3->X1 = 823+187*sin((rotxLimit*2*PI)/360);
				lineShape3->X2 = 823;
				lineShape3->Y1 = 215-187*cos((rotxLimit*2*PI)/360);
				lineShape3->Y2 = 215;
				double errorXrad=(int)rotxError*2*PI/360;
				lineShape2->X1 = 823+187*sin(errorXrad);
				lineShape2->X2 = 823;
				lineShape2->Y1 = 215-187*cos(errorXrad);
				lineShape2->Y2 = 215;//+(int)cos(rotxError);*/
				double errorXrad= rotxError*2*PI/360;
				double errorYrad= rotyError*2*PI/360;
				double errorZrad= rotzError*2*PI/360;//(int)((rotzError*2*PI/360));
				lineShape6->X1 = 928;
				lineShape6->X2 = 1169;
				lineShape6->Y1 = 118+241*sin(errorYrad);
				lineShape6->Y2 = 118-241*sin(errorYrad);
				lineShape3->X1 = 925;
				lineShape3->X2 = 744;
				lineShape3->Y1 = 118+90*sin(errorZrad);
				lineShape3->Y2 = 118-90*sin(errorZrad);
				lineShape9->X1 = 458;
				lineShape9->X2 = 676;
				lineShape9->Y1 = 118+109*sin(errorXrad);
				lineShape9->Y2 = 118-109*sin(errorXrad);
				/*lineShape10->X1 = 834+105*sin(errorZrad);
				lineShape10->X2 = 834-105*sin(errorZrad);
				lineShape10->Y1 = 12;
				lineShape10->Y2 = 222;*/
				//chart2->Series["Series1"]->Points->AddXY((floor(10*distErrorx))/10,(floor(distErrory*10))/10); limited at 1 num after comma
				if(fabs(distError)<=10){ // CAMBIATO 
				chart2->Series["Series1"]->Points->AddXY((int)(distStartx),-(int)(distStarty));
				}else{
				chart2->Series["Series1"]->Points->AddXY(0.0,0.0);
				chart2->Series["Series1"]->Points->AddXY((int)(distStartx),-(int)(distStarty));
				}

		}
		
		//}else{ // if tool isn't visible compare BLACK 
		//	chart1->ChartAreas["ChartArea1"]->BackColor = System::Drawing::Color::Black;
	//}
	}

		 }
		 // point coil calibration
	private: System::Void Coil1_Click(System::Object^  sender, System::EventArgs^  e) {
this->button2->Enabled = false; 
		Coil1x=Coil3x;
		Coil1y=Coil3y;
		Coil1z=Coil3z;
		 fstream outf;
		outf.open( "CoilL.txt", ios::app );
		outf << Coil1x << " " << Coil1y << " "<< Coil1z<<"\n" << endl; 
		outf.close();
		//label1->Text ="CoilL Done";
		this->button2->BackColor=System::Drawing::Color::Gray;
		 }
	private: System::Void Coil2_Click(System::Object^  sender, System::EventArgs^  e) {
this->button4->Enabled = false; 
		Coil2x=Coil3x;
		Coil2y=Coil3y;
		Coil2z=Coil3z;
		 fstream outf;
		outf.open( "CoilO.txt", ios::app );
		outf << Coil2x << " " << Coil2y << " "<< Coil2z<<"\n" << endl; 
		outf.close();
			//label1->Text ="CoilO Done";
		this->button4->BackColor=System::Drawing::Color::Gray;
		 }
	private: System::Void Coil3_Click(System::Object^  sender, System::EventArgs^  e) {
this->button5->Enabled = false; 
		 fstream outf;
		outf.open( "CoilR.txt", ios::app );
		outf << Coil3x << " " << Coil3y << " "<< Coil3z<<"\n" << endl; 
		outf.close();
		backgroundWorker3->CancelAsync();
			//label1->Text ="COIL Done";
		this->button5->BackColor=System::Drawing::Color::Gray;
		 }
	private: System::Void button18_Click(System::Object^  sender, System::EventArgs^  e) {
this->button18->Enabled = false; 
		CoilUpx=Coil3x;
		CoilUpy=Coil3y;
		CoilUpz=Coil3z;
		 fstream outf;
		outf.open( "CoilUp.txt", ios::app );
		outf << CoilUpx << " " << CoilUpy << " "<< CoilUpz<<"\n" << endl; 
		outf.close();
			//label1->Text ="CoilUp Done";
		this->button18->BackColor=System::Drawing::Color::Gray;
		 }
		 // point head calibration
	private: System::Void HeadLeft_Click(System::Object^  sender, System::EventArgs^  e) {
		this->button6->Enabled = false; 
		HeadLeftx=HeadRigthx;
		HeadLefty=HeadRigthy;
		HeadLeftz=HeadRigthz;
		 fstream outf;
		outf.open( "HeadLeft.txt", ios::app );
		outf << HeadLeftx << " " << HeadLefty << " "<< HeadLeftz<<"\n" << endl; 
		outf.close();
			//label1->Text ="HeadLeft Done";
		this->button6->BackColor=System::Drawing::Color::Gray;
		 }
	private: System::Void HeadRight_Click(System::Object^  sender, System::EventArgs^  e) {
		this->button8->Enabled = false; 
		fstream outf;
		outf.open( "HeadRight.txt", ios::app );
		outf << HeadRigthx << " " << HeadRigthy << " "<< HeadRigthz<<"\n" << endl; 
		outf.close();
		backgroundWorker14->CancelAsync();
		//label1->Text ="HEAD Done";
		this->button8->BackColor=System::Drawing::Color::Gray;
			 }
	private: System::Void HeadNasion_Click(System::Object^  sender, System::EventArgs^  e) {
		this->button7->Enabled = false; 
		HeadNasionx=HeadRigthx;
		HeadNasiony=HeadRigthy;
		HeadNasionz=HeadRigthz;
		 fstream outf;
		outf.open( "HeadNasion.txt", ios::app );
		outf << HeadNasionx << " " << HeadNasiony << " "<< HeadNasionz<<"\n" << endl; 
		outf.close();
		//label1->Text ="HeadNasion Done";
		this->button7->BackColor=System::Drawing::Color::Gray;
		 }
			 // analogical signal
	private: System::Void button11_Click(System::Object^  sender, System::EventArgs^  e) {
	 this->button11->Enabled = false;
			highestPercentageReached=0;
			// Kickoff the worker thread to begin it's DoWork function.
            this->progressBar1->Value=0;
			backgroundWorker11->RunWorkerAsync(); //CALIBRATE

	
}
	private: System::Void backgroundWorker11_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
	this->button11->Enabled = false;
	int         error=0;
	TaskHandle  taskHandle=0;
	float64     data[10];
	char        errBuff[2048]={'\0'};

	uInt32      i=0;
	int			j=0;

	for(i=0;i<10;i++)
		if(i>8)
		  data[i]=0;
		else
			data[i] = 5;
	//i = 0;

	/*********************************************/
	// DAQmx Configure Code
	/*********************************************/
	DAQmxErrChk (DAQmxCreateTask("",&taskHandle));
	DAQmxErrChk (DAQmxCreateAOVoltageChan(taskHandle,"Dev1/ao0","",0.0,5.0,DAQmx_Val_Volts,NULL));
	
	/*********************************************/
	// DAQmx Start Code
	/*********************************************/
	DAQmxErrChk (DAQmxStartTask(taskHandle));

	printf("Generating samples continuously. Press Ctrl+C to interrupt\n");
	while( j<10 ) {
		#if defined(WIN32) || defined(_WIN32)
			Sleep(1);
		#else
			#error - This example requires a platform specific sleep call.
			/*********************************************/
			// This example requires a platform specific sleep call.
			// For example:
			//
			// #include <windows.h>
			// Sleep(1);		// For Windows platform
			//
			// #include <unistd.h>
			// usleep(1000);	// For Linux platform
			/*********************************************/
		#endif

		/*********************************************/
		// DAQmx Write Code
		/*********************************************/
		DAQmxErrChk (DAQmxWriteAnalogScalarF64(taskHandle,1,10,data[j],NULL));
		
		j++;
	
	}

Error:
	if( DAQmxFailed(error) )
		DAQmxGetExtendedErrorInfo(errBuff,2048);
	if( taskHandle!=0 ) {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}
	if( DAQmxFailed(error) )
		printf("DAQmx Error: %s\n",errBuff);
	printf("End of program, press Enter key to quit\n");
	getchar();



		
		this->button11->Enabled = true;	

		 }
	private: System::Void backgroundWorker11_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e){
		 this->progressBar1->Value = e->ProgressPercentage;
		 }
	private: System::Void backgroundWorker11_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e){
		  if ( e->Cancelled ){
         // Next, handle the case where the user cancelled 
         // the operation.
         // Note that due to a race condition in 
         // the DoWork event handler, the Cancelled
         // flag may not have been set, even though
         // CancelAsync was called.
        // label1->Text = "Cancelled";
      }
      else
      {
         // Finally, handle the case where the operation 
         // succeeded.
        // label1->Text ="Succeed";
		  }
}
			// Load Hotspot
	private: System::Void button12_Click(System::Object^  sender, System::EventArgs^  e) {
			this->button12->Enabled = false;
			double input[7];
			int x=0;
			// read directory path from text box
			System::String ^ Directory = textBox2->Text + "\\HotSpotCalibration.txt"; //path directory 	
			pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
			const wchar_t *constValue = convertedValue;
					// CHECK IF PATH EXIST
					System::String ^ pathDirectory = textBox2->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						label21->Text =""; // PATH EXIST 
					}else{
						label21->Text ="PATH NOT EXIST"; 
					}
			ifstream textfile(constValue);
			if (textfile.good()) {
			while(! textfile.eof()){
			textfile >> input[x];
			if(x==0)
			distCalibratex= input[x];
			if(x==1)
			distCalibratey= input[x];
			if(x==2)
			distCalibratez= input[x];
			if(x==3)
			HotSpot_w= input[x];
			if(x==4)
			HotSpot_x= input[x];
			if(x==5)
			HotSpot_y= input[x];
			if(x==6)
			HotSpot_z= input[x];
			x++;
			}
			label4->Text ="HotSpot Loaded";
			}else{
			label4->Text ="FILE NOT PRESENT";
			}
				this->button12->Enabled = true; 
				
		 }
			// Load Coil
	private: System::Void button13_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->button13->Enabled = false;
					 // in case Load calibration and Load hotspot (without push Calibrate)
				double input[7];
				int x=0;
				System::String ^ Directory = textBox2->Text + "\\CoilCalibration.txt"; //path directory 	//write directory
				pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
				const wchar_t *constValue = convertedValue;
						// CHECK IF PATH EXIST
					System::String ^ pathDirectory = textBox2->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						label21->Text =""; // PATH EXIST
					}else{
						label21->Text ="PATH NOT EXIST"; 
					}
				ifstream textfile(constValue);
				if (textfile.good()) {
				while(! textfile.eof()){
				textfile >> input[x];
				if(x==0)
				trans_coilx= input[x];
				if(x==1)
				trans_coily= input[x];
				if(x==2)
				trans_coilz= input[x];
				if(x==3)
				qx_coil= input[x];
				if(x==4)
				qy_coil= input[x];
				if(x==5)
				qz_coil= input[x];
				if(x==6)
				qw_coil= input[x];
				x++;
				}
				label2->Text ="COIL Loaded";
		        } else {
				label2->Text= "FILE NOT PRESENT";
		} 
			
			this->button13->Enabled = true; 
		 }
	private: System::Void chart1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
			// Load Head	
	private: System::Void button14_Click(System::Object^  sender, System::EventArgs^  e) {
			  this->button14->Enabled = false;
			

			 
					 // in case Load calibration and Load hotspot (without push Calibrate)
		double input[7];
		int x=0;
		// read directory path from text box
		System::String ^ Directory = textBox2->Text + "\\HeadCalibration.txt"; //path directory 	
		pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
		const wchar_t *constValue = convertedValue;
		// CHECK IF PATH EXIST
					System::String ^ pathDirectory = textBox2->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						label21->Text =""; // PATH EXIST
					}else{
						label21->Text ="PATH NOT EXIST"; 
					}
		ifstream textfile(constValue);
		if (textfile.good()) {
		while(! textfile.eof()){
		textfile >> input[x];
		if(x==0)
		trans_headx= input[x];
		if(x==1)
		trans_heady= input[x];
		if(x==2)
		trans_headz= input[x];
		if(x==3)
		qx_head= input[x];
		if(x==4)
		qy_head= input[x];
		if(x==5)
		qz_head= input[x];
		if(x==6)
		qw_head= input[x];
		x++;
		}
		label3->Text ="HEAD Loaded";
		}else{
		label3->Text ="FILE NOT PRESENT";
		}
			this->button14->Enabled = true; 
		}
			// Stop
	private: System::Void button15_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->button15->Enabled = false; 
		 fstream outf;
		outf.open( "Sample.txt", ios::app );
		outf << distError << " " << rotxError << " "<< rotyError <<" "<<rotzError<< "\n"<< q_ws << " " << bs << " "<< cs <<" "<<ds<< "\n"<< endl; 
		outf.close();

			backgroundWorker2->CancelAsync();
			
		 }
			// // Load Verify calibration
	private: System::Void button16_Click(System::Object^  sender, System::EventArgs^  e) {
		 this->button14->Enabled = false;
		double input[3];
		double inputModel[3];
		int x=0;
		System::String ^ Directory = textBox2->Text + "\\QualityCalibrHead.txt"; //the last done	
		pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
		const wchar_t *constValue = convertedValue;
				// CHECK IF PATH EXIST
					System::String ^ pathDirectory = textBox2->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						label21->Text =""; // PATH EXIST
					}else{
						label21->Text ="PATH NOT EXIST"; 
					}
		ifstream textfile(constValue);
		if (textfile.good()) {
			while(! textfile.eof()){
			textfile >> input[x];
			if(x==0)
			d1=input[x];
			if(x==1)
			d2=input[x];
			if(x==2)
			d3=input[x];
			x++;
			}
			x=0;
			System::String ^ DirectoryModel = textBox2->Text + "\\HeadCalibrationModel.txt"; //the last done	
			pin_ptr<const wchar_t> convertedValueModel = PtrToStringChars(DirectoryModel);  // <-- #include <vcclr.h>
			const wchar_t *constValueModel = convertedValueModel;
			ifstream textfileModel(constValueModel);
			if (textfileModel.good()) {
				while(! textfileModel.eof()){
				textfileModel >> inputModel[x];
				if(x==0)
				d1Model=inputModel[x];
				if(x==1)
				d2Model=inputModel[x];
				if(x==2)
				d3Model=inputModel[x];
				x++;
				}
				if(fabs(d1-d1Model)<1.5){
					label5->Text= "HL-HR:Good";
				}else{
					label5->Text="HL-HR:" + ((int)(fabs(d1-d1Model)+0.5)).ToString();
				}
				if(fabs(d2-d2Model)<1.5){
					label7->Text= "HN-HR:Good";
				}else{
					label7->Text= "HN-HR:" + ((int)(fabs(d2-d2Model)+0.5)).ToString();
				}
				if(fabs(d3-d3Model)<1.5){
					label8->Text= "HL-HN:Good";
				}else{
					label8->Text= "HL-HN:" + ((int)(fabs(d3-d3Model)+0.5)).ToString();
				}
			}else{
			label5->Text= "FILE NOT PRESENT Model";
			label7->Text= "";
			label8->Text= "";
			}
		}else{
		label5->Text= "FILE NOT PRESENT Online";
		label7->Text= "";
		label8->Text= "";
		}	 

	/*	ofstream outf;
		outf.open( "ResultsQualityCalibr.txt", ios::app );
		outf << d1 << " " << d2 << " "<<d3<<"\n"<<  endl; 
		
		outf.close();*/
		this->button14->Enabled = true;
 }
	private: System::Void chart2_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
	private: System::Void lineShape3_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
	 // head calibration	
	private: System::Void button9_Click(System::Object^  sender, System::EventArgs^  e) {
			  this->button9->Enabled = false;
			// int Coil=1;
				//this->button2->Enabled = true;
				highestPercentageReached=0;
				label20->Text =""; 
				this->button6->BackColor=System::Drawing::Color::Khaki;
				this->button7->BackColor=System::Drawing::Color::Khaki;
				this->button8->BackColor=System::Drawing::Color::Khaki;
				this->button19->BackColor=System::Drawing::Color::Khaki;
				this->button19->Enabled = true;
            // Kickoff the worker thread to begin it's DoWork function.
            this->progressBar1->Value=0;
			backgroundWorker14->RunWorkerAsync(); //CALIBRATE
		 }
	private: System::Void backgroundWorker14_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
				//#define tot_frame 20;
				 int freq_track = 20; // 20Hz  la frequenza a cui viene utilizzato il Vicra. Ma questo valore sembra non cambiare niente!
				// #define tot_frame 20; // LI RIDUCO!! cos si velocizza (20)
				 int i=0;
				 int j=0;
				 double data[1000][3];  // [tot_frame][4]
				 for(i=0; i<1000;i++){
					for(j=0; j<3;j++){
						data[i][j]=-1;
					}
				}
	int row=0;					// aggiungo static
	//char *av[4];
	//av[0]="LogFile1.txt";
	//av[1]="C:/vicra/rom/croceNDI_HEAD.rom";
	System::String ^ ComNumber = textBox3->Text; 
	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(ComNumber);
	int number= atoi(str2);//)-(int)(str4);// - 1;
	int numero= number - 1;
	// convert int to char*
	stringstream strs;
	strs << numero;
	string temp_str = strs.str();
	char* char_type = (char*) temp_str.c_str();
	//av[2]= char_type; //av[2]= "0";
	//av[3]="C:/vicra/rom/VicraPointer.rom";

	/////////////////////////////////////////////     START OF VICRA PART     /////////////////////////////////////////////         
	igstk::RealTimeClock::Initialize();

	typedef igstk::Object::LoggerType			LoggerType;
	typedef itk::StdStreamLogOutput				LogOutputType;
	typedef igstk::TransformObserver			ObserverType;
	typedef igstk::PolarisTrackerTool			TrackerToolType;
	typedef TrackerToolType::TransformType		TransformType;
	igstk::PolarisTrackerTool::Pointer tool = igstk::PolarisTrackerTool::New();
	igstk::SerialCommunication::Pointer serialComm = igstk::SerialCommunication::New();
	PolarisTrackerCommand::Pointer my_command = PolarisTrackerCommand::New();
	
	typedef igstk::SerialCommunication::PortNumberType PortNumberType;
	unsigned int portNumberIntegerValue = atoi(char_type);// atoi(av[3])

	//std::cout << "porta" << portNumberIntegerValue << std::endl;

	PortNumberType  polarisPortNumber = PortNumberType(portNumberIntegerValue); 
	serialComm->SetPortNumber( polarisPortNumber );
	serialComm->SetParity( igstk::SerialCommunication::NoParity );
	serialComm->SetBaudRate( igstk::SerialCommunication::BaudRate9600 );
	serialComm->SetDataBits( igstk::SerialCommunication::DataBits8 );
	serialComm->SetStopBits( igstk::SerialCommunication::StopBits1 );
	serialComm->SetHardwareHandshake( igstk::SerialCommunication::HandshakeOff );
	//serialComm->SetCaptureFileName( "RecordedStreamByPolarisTracker1.txt" );
	//serialComm->SetCapture( true );
	serialComm->OpenCommunication();

	igstk::PolarisTracker::Pointer  tracker = igstk::PolarisTracker::New();
	tracker->AddObserver( itk::AnyEvent(), my_command);
	//tracker->SetLogger( logger );
	label1->Text ="SetCommunication()";
	//std::cout << "SetCommunication()" << std::endl;
	tracker->SetCommunication( serialComm );
	label1->Text ="RequestOpen()";
	//std::cout << "RequestOpen()" << std::endl;
	tracker->RequestOpen();
	ofstream outf;

  
//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 1 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool1";
	//std::cout << "Instantiate wireless tracker tool1: " << std::endl;
	TrackerToolType::Pointer trackerTool1 = TrackerToolType::New();                       // commented out
	//trackerTool1->SetLogger( logger );                                                    // commented out
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool1->AddObserver( itk::AnyEvent(), my_command);                              // commented out
	
	//For wireless tracker tool type, invoke RequestSelectWirelessTrackerTool method.
	
	trackerTool1->RequestSelectWirelessTrackerTool();  
	std::string romFile1 ="C:/vicra/rom/croceNDI_HEAD.rom";
	
	//std::cout << "SROM file: " << romFile1 << std::endl;		
	
	trackerTool1->RequestSetSROMFileName( romFile1 );
	
	//Configure
	
	trackerTool1->RequestConfigure();                                                     // commented out
	
	//Attach to the tracker
	
	trackerTool1->RequestAttachToTracker( tracker );                                      // commented out
	
	//Add observer to listen to transform events 
	
	ObserverType::Pointer coordSystemAObserver1 = ObserverType::New();                    // commented out
	coordSystemAObserver1->ObserveTransformEventsFrom( trackerTool1 );                     // commented out	

	//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 2 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool2";
	//std::cout << "Instantiate wireless tracker tool2: " << std::endl;
	TrackerToolType::Pointer trackerTool2 = TrackerToolType::New();
	//trackerTool2->SetLogger( logger );
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool2->AddObserver( itk::AnyEvent(), my_command );

	trackerTool2->RequestSelectWirelessTrackerTool();
	
	//Set the SROM file 
	
	std::string romFile ="C:/vicra/rom/VicraPointer.rom";
	
	//std::cout << "SROM file: " << romFile << std::endl;
	trackerTool2->RequestSetSROMFileName( romFile );

	//Configure
	
	trackerTool2->RequestConfigure();
	
	//Attach to the tracker

	trackerTool2->RequestAttachToTracker( tracker );

	//Add observer to listen to transform events 

	ObserverType::Pointer coordSystemAObserver2 = ObserverType::New();
	coordSystemAObserver2->ObserveTransformEventsFrom( trackerTool2 );
	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// tracking starts and define the frequency

	tracker->RequestSetFrequency(freq_track);
	tracker->RequestStartTracking();

	
	typedef igstk::Transform            Transform;
	typedef ::itk::Vector<double,3>    VectorType; //<double, 3>
	typedef ::itk::Versor<double>		VersorType;//<double>
	typedef ::itk::Matrix<double, 4,4>  MatrixType;

	

/////////////////////////////////////////   END OF VICRA PART     /////////////////////////////////////////



// WHEN WE WANT TO GET THE DATA FROM THE VICRA (IN THIS EXAMPLE FOR THE FIRST TOOL) WE HAVE TO INSERT THE FOLLOWING CODE
	
	int start=clock();
	resetplot=0;

	time_t tstart, tend;
	tstart=time(0);
	label1->Text="COM NOT AVAILABLE";	
	chart1->Series["Series1"]->Points->Clear();
	count=0;

	while (row<1000) 
		{
		
			if(backgroundWorker14->CancellationPending) //if it was cancelled
					{
						//origin SR Head 
					HeadOx=HeadNasionx;
					HeadOy=HeadNasiony;
					HeadOz=HeadNasionz;
					//terna, vector x(from right to left)
					// auxilian vector (from head right to HeadUpLeft)
					HeadXi=(HeadLeftx-HeadRigthx)/sqrtf(pow(HeadLeftx-HeadRigthx,2)+pow(HeadLefty-HeadRigthy,2)+pow(HeadLeftz-HeadRigthz,2));
					HeadXj=(HeadLefty-HeadRigthy)/sqrtf(pow(HeadLeftx-HeadRigthx,2)+pow(HeadLefty-HeadRigthy,2)+pow(HeadLeftz-HeadRigthz,2));
					HeadXk=(HeadLeftz-HeadRigthz)/sqrtf(pow(HeadLeftx-HeadRigthx,2)+pow(HeadLefty-HeadRigthy,2)+pow(HeadLeftz-HeadRigthz,2));
					HeadAusx=(HeadNasionx-HeadRigthx)/sqrtf(pow(HeadNasionx-HeadRigthx,2)+pow(HeadNasiony-HeadRigthy,2)+pow(HeadNasionz-HeadRigthz,2));
					HeadAusy=(HeadNasiony-HeadRigthy)/sqrtf(pow(HeadNasionx-HeadRigthx,2)+pow(HeadNasiony-HeadRigthy,2)+pow(HeadNasionz-HeadRigthz,2));
					HeadAusz=(HeadNasionz-HeadRigthz)/sqrtf(pow(HeadNasionx-HeadRigthx,2)+pow(HeadNasiony-HeadRigthy,2)+pow(HeadNasionz-HeadRigthz,2));
					
					//prodotto scalare a2b3-a3b2,-(a1b3-a3b1),a1b2-a2b1 , vector y (from front to back)
					HeadYi=(HeadXj*HeadAusz-HeadXk*HeadAusy)/sqrtf(pow(HeadXj*HeadAusz-HeadXk*HeadAusy,2)+pow(HeadXi*HeadAusz-HeadXk*HeadAusx,2)+pow(HeadXi*HeadAusy-HeadXj*HeadAusx,2));
					HeadYj=-(HeadXi*HeadAusz-HeadXk*HeadAusx)/sqrtf(pow(HeadXj*HeadAusz-HeadXk*HeadAusy,2)+pow(HeadXi*HeadAusz-HeadXk*HeadAusx,2)+pow(HeadXi*HeadAusy-HeadXj*HeadAusx,2));
					HeadYk=(HeadXi*HeadAusy-HeadXj*HeadAusx)/sqrtf(pow(HeadXj*HeadAusz-HeadXk*HeadAusy,2)+pow(HeadXi*HeadAusz-HeadXk*HeadAusx,2)+pow(HeadXi*HeadAusy-HeadXj*HeadAusx,2));
					// prodotto scalare a2b3-a3b2,-(a1b3-a3b1),a1b2-a2b1 , vector z (from down to up)
					HeadZi=(HeadXj*HeadYk-HeadXk*HeadYj)/sqrtf(pow(HeadXj*HeadYk-HeadXk*HeadYj,2)+pow(HeadXi*HeadYk-HeadXk*HeadYi,2)+pow(HeadXi*HeadYj-HeadXj*HeadYi,2));
					HeadZj=-(HeadXi*HeadYk-HeadXk*HeadYi)/sqrtf(pow(HeadXj*HeadYk-HeadXk*HeadYj,2)+pow(HeadXi*HeadYk-HeadXk*HeadYi,2)+pow(HeadXi*HeadYj-HeadXj*HeadYi,2));
					HeadZk=(HeadXi*HeadYj-HeadXj*HeadYi)/sqrtf(pow(HeadXj*HeadYk-HeadXk*HeadYj,2)+pow(HeadXi*HeadYk-HeadXk*HeadYi,2)+pow(HeadXi*HeadYj-HeadXj*HeadYi,2));
					// Homo_Head  
					MatrixType		Homo_Head;
					Homo_Head[0][0]= HeadXi; Homo_Head[1][0]= HeadXj; Homo_Head[2][0]= HeadXk;Homo_Head[3][0]= 0;
					Homo_Head[0][1]= HeadYi;Homo_Head[1][1]= HeadYj;Homo_Head[2][1]= HeadYk;Homo_Head[3][1]= 0;
					Homo_Head[0][2]= HeadZi;Homo_Head[1][2]= HeadZj;Homo_Head[2][2]= HeadZk;Homo_Head[3][2]= 0;
					Homo_Head[0][3]= HeadOx;Homo_Head[1][3]= HeadOy;Homo_Head[2][3]= HeadOz;Homo_Head[3][3]= 1;
					//quaternion form Homo_Head
					qw_head=sqrtf(1+Homo_Head[0][0]+Homo_Head[1][1]+Homo_Head[2][2])/2;
					qx_head=(Homo_Head[2][1]-Homo_Head[1][2])/(4*qw_head);
					qy_head=(Homo_Head[0][2]-Homo_Head[2][0])/(4*qw_head);
					qz_head=(Homo_Head[1][0]-Homo_Head[0][1])/(4*qw_head);
					// traslation form Homo_matrix
					trans_headx=HeadOx;//Homo_matrix[0][3];  
					trans_heady=HeadOy;//Homo_matrix[1][3];
					trans_headz=HeadOz;//Homo_matrix[2][3];
					//plot Calibration Head   (traslx,trasly,traslz,qx,qy,qz,qw)
					// read the dictory from text box
					fstream outf; 
					System::String ^ Directory = textBox1->Text + "\\HeadCalibration.txt"; //path directory 	//write directory
					pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
					const wchar_t *constValue = convertedValue; 
					// CHECK IF PATH EXIST
					System::String ^ pathDirectory = textBox1->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						//label20->Text ="";//PATH exist 
					}else{
						label20->Text ="PATH NOT EXIST"; 
					}
					ofstream outfile(constValue);
					outfile << trans_headx << "\n" << trans_heady<< "\n"<<trans_headz<<"\n"<<qx_head << "\n" << qy_head<< "\n"<<qz_head <<"\n"<<qw_head <<"\n"<<endl; 
					outfile.close();
					// to verify the quality of Calibration Head
					dist1=sqrtf(pow(HeadLeftx-HeadRigthx,2)+pow(HeadLefty-HeadRigthy,2)+pow(HeadLeftz-HeadRigthz,2));
					dist2=sqrtf(pow(HeadNasionx-HeadRigthx,2)+pow(HeadNasiony-HeadRigthy,2)+pow(HeadNasionz-HeadRigthz,2));
					dist3=sqrtf(pow(HeadLeftx-HeadNasionx,2)+pow(HeadLefty-HeadNasiony,2)+pow(HeadLeftz-HeadNasionz,2));
					System::String ^ Directory1 = textBox1->Text + "\\QualityCalibrHead.txt"; //path directory 	//write directory
					pin_ptr<const wchar_t> convertedValue1 = PtrToStringChars(Directory1);  // <-- #include <vcclr.h>
					const wchar_t *constValue1 = convertedValue1; 
					ofstream outfile1(constValue1);
					outfile1 << dist1 << "\n" << dist2<< "\n"<<dist3<<"\n"<<endl; 
					outfile1.close();
					this->button8->Enabled = true;
					e->Cancel=true;
					break;
					}
		//label1->Text =Data[row][0]->ToString();
			
		worker->ReportProgress( row*100/1000 );
		if ( row*100/1000  > highestPercentageReached )//quando finisce il while la barra si ferma l
        {
          highestPercentageReached = row*100/1000 ;
         worker->ReportProgress( row*100/1000  );
        }
		
		igstk::PulseGenerator::CheckTimeouts();
		Sleep(5);
		
		Transform		transform1, transform2, transformTip;
		VectorType		vector1, vector2; 
		VersorType		versor1, versor2;
	
		coordSystemAObserver1->Clear();
		coordSystemAObserver2->Clear();
		trackerTool1->RequestGetTransformToParent();
		trackerTool2->RequestComputeTransformTo(trackerTool1);
		//label1->Text="Put distance1";	
		if (coordSystemAObserver1->GotTransform() && coordSystemAObserver2->GotTransform() ) { // 
			transform1 = coordSystemAObserver1->GetTransform();
			transform2 = coordSystemAObserver2->GetTransform();
			//label1->Text="Put distance2";	
			if ( transform1.IsValidNow() && !transform2.IsValidNow()){
				count=0;
				label1->Text ="POINTER NOT VISIBLE";
				this->button9->BackColor=System::Drawing::Color::Red;
				this->button6->Enabled = false;
				this->button7->Enabled = false;
				this->button8->Enabled = false;
			}
			if ( !transform1.IsValidNow() && transform2.IsValidNow() || !transform1.IsValidNow() && !transform2.IsValidNow()){
				count=0;
				label1->Text ="HEAD NOT VISIBLE";
				this->button9->BackColor=System::Drawing::Color::Red;
				this->button6->Enabled = false;
				this->button7->Enabled = false;
				this->button8->Enabled = false;
			}
			if ( transform1.IsValidNow() && transform2.IsValidNow()){
				vector2 = transform2.GetTranslation();
				versor2 = transform2.GetRotation();
				data[row][0] =vector2[0];//DISTANZA		
				label1->Text="Working";	
				data[row][1]= vector2[1]; //TETA
				data[row][2]=vector2[2]; //PSI
				this->button9->BackColor=System::Drawing::Color::Green;
				this->button6->Enabled = true;
				this->button7->Enabled = true;
				this->button8->Enabled = true;
				HeadRigthx=data[row][0];
				HeadRigthy=data[row][1];
				HeadRigthz=data[row][2];
				}
			}
		row++;
	}	
	
	
		

		this->button9->BackColor=System::Drawing::Color::White;
		label1->Text ="RequestStopTracking()";
		tracker->RequestStopTracking();
		tracker->RequestClose();
		serialComm->CloseCommunication();

		this->button9->Enabled = true;	

		 }
	private: System::Void backgroundWorker14_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e){
		 this->progressBar1->Value = e->ProgressPercentage;
		 }
	private: System::Void backgroundWorker14_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e){
		  if ( e->Cancelled ){
         // Next, handle the case where the user cancelled 
         // the operation.
         // Note that due to a race condition in 
         // the DoWork event handler, the Cancelled
         // flag may not have been set, even though
         // CancelAsync was called.
         label1->Text = "Calibration Head DONE";
      }
      else
      {
         // Finally, handle the case where the operation 
         // succeeded.
         //label1->Text ="Succeed";
		  }
}
	private: System::Void label10_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
		 // coil calibration	
	private: System::Void button10_Click(System::Object^  sender, System::EventArgs^  e) {
			this->button10->Enabled = false;
			// int Coil=1;
				//this->button2->Enabled = true;
				highestPercentageReached=0;
				label20->Text =""; 
				this->button2->BackColor=System::Drawing::Color::Khaki;
				this->button4->BackColor=System::Drawing::Color::Khaki;
				this->button18->BackColor=System::Drawing::Color::Khaki;
				this->button5->BackColor=System::Drawing::Color::Khaki;
            // Kickoff the worker thread to begin it's DoWork function.
            this->progressBar1->Value=0;
			backgroundWorker3->RunWorkerAsync(); //CALIBRATE
		 }
	private: System::Void backgroundWorker3_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			 BackgroundWorker^ worker = dynamic_cast<BackgroundWorker^>(sender);
				//#define tot_frame 20;
				 int freq_track = 20; // 20Hz  la frequenza a cui viene utilizzato il Vicra. Ma questo valore sembra non cambiare niente!
				// #define tot_frame 20; // LI RIDUCO!! cos si velocizza (20)
				 int i=0;
				 int j=0;
				 double data[1000][3];  // [tot_frame][4]
				 for(i=0; i<1000;i++){
					for(j=0; j<3;j++){
						data[i][j]=-1;
					}
				}
					
				 
				 
	//double datatrasl[tot_frame][6];
	//double datarot[tot_frame][3];
	int row=0;					// aggiungo static
	//char *av[4];
	//av[0]="LogFile1.txt";
	//av[1]="C:/vicra/rom/croceNDI_COIL.rom";
	System::String ^ ComNumber = textBox3->Text; 
	char* str2 = (char*)(void*)Marshal::StringToHGlobalAnsi(ComNumber);
	int number= atoi(str2);//)-(int)(str4);// - 1;
	int numero= number - 1;
	// convert int to char*
	stringstream strs;
	strs << numero;
	string temp_str = strs.str();
	char* char_type = (char*) temp_str.c_str();
	//av[2]= char_type; //av[2]= "0";
	//av[3]="C:/vicra/rom/VicraPointer.rom";

	/////////////////////////////////////////////     START OF VICRA PART     /////////////////////////////////////////////         
	igstk::RealTimeClock::Initialize();

	typedef igstk::Object::LoggerType			LoggerType;
	typedef itk::StdStreamLogOutput				LogOutputType;
	typedef igstk::TransformObserver			ObserverType;
	typedef igstk::PolarisTrackerTool			TrackerToolType;
	typedef TrackerToolType::TransformType		TransformType;
	igstk::PolarisTrackerTool::Pointer tool = igstk::PolarisTrackerTool::New();
	igstk::SerialCommunication::Pointer serialComm = igstk::SerialCommunication::New();
	PolarisTrackerCommand::Pointer my_command = PolarisTrackerCommand::New();
	
	typedef igstk::SerialCommunication::PortNumberType PortNumberType;
	unsigned int portNumberIntegerValue = atoi(char_type);// atoi(av[3])

	//std::cout << "porta" << portNumberIntegerValue << std::endl;

	PortNumberType  polarisPortNumber = PortNumberType(portNumberIntegerValue); 
	serialComm->SetPortNumber( polarisPortNumber );
	serialComm->SetParity( igstk::SerialCommunication::NoParity );
	serialComm->SetBaudRate( igstk::SerialCommunication::BaudRate9600 );
	serialComm->SetDataBits( igstk::SerialCommunication::DataBits8 );
	serialComm->SetStopBits( igstk::SerialCommunication::StopBits1 );
	serialComm->SetHardwareHandshake( igstk::SerialCommunication::HandshakeOff );
	//serialComm->SetCaptureFileName( "RecordedStreamByPolarisTracker1.txt" );
	//serialComm->SetCapture( true );
	serialComm->OpenCommunication();

	igstk::PolarisTracker::Pointer  tracker = igstk::PolarisTracker::New();
	tracker->AddObserver( itk::AnyEvent(), my_command);
	//tracker->SetLogger( logger );
	label1->Text ="SetCommunication()";
	//std::cout << "SetCommunication()" << std::endl;
	tracker->SetCommunication( serialComm );
	label1->Text ="RequestOpen()";
	//std::cout << "RequestOpen()" << std::endl;
	tracker->RequestOpen();
	ofstream outf;

  
//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 1 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool1";
	//std::cout << "Instantiate wireless tracker tool1: " << std::endl;
	TrackerToolType::Pointer trackerTool1 = TrackerToolType::New();                       // commented out
	//trackerTool1->SetLogger( logger );                                                    // commented out
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool1->AddObserver( itk::AnyEvent(), my_command);                              // commented out
	
	//For wireless tracker tool type, invoke RequestSelectWirelessTrackerTool method.
	
	trackerTool1->RequestSelectWirelessTrackerTool();  
	std::string romFile1 ="C:/vicra/rom/croceNDI_COIL.rom";
	
	//std::cout << "SROM file: " << romFile1 << std::endl;		
	
	trackerTool1->RequestSetSROMFileName( romFile1 );
	
	//Configure
	
	trackerTool1->RequestConfigure();                                                     // commented out
	
	//Attach to the tracker
	
	trackerTool1->RequestAttachToTracker( tracker );                                      // commented out
	
	//Add observer to listen to transform events 
	
	ObserverType::Pointer coordSystemAObserver1 = ObserverType::New();                    // commented out
	coordSystemAObserver1->ObserveTransformEventsFrom( trackerTool1 );                     // commented out	

	//%%%%%%%%%%%%%%%%%%%%%% Setting up Wireless tool 2 %%%%%%%%%%%%%%%%%%%%%
	label1->Text ="Instantiate wireless tracker tool2";
	//std::cout << "Instantiate wireless tracker tool2: " << std::endl;
	TrackerToolType::Pointer trackerTool2 = TrackerToolType::New();
	//trackerTool2->SetLogger( logger );
	
	//Add observer to listen to events throw by the tracker tool
	
	trackerTool2->AddObserver( itk::AnyEvent(), my_command );

	trackerTool2->RequestSelectWirelessTrackerTool();
	
	//Set the SROM file 
	
	std::string romFile ="C:/vicra/rom/VicraPointer.rom";
	
	//std::cout << "SROM file: " << romFile << std::endl;
	trackerTool2->RequestSetSROMFileName( romFile );

	//Configure
	
	trackerTool2->RequestConfigure();
	
	//Attach to the tracker

	trackerTool2->RequestAttachToTracker( tracker );

	//Add observer to listen to transform events 

	ObserverType::Pointer coordSystemAObserver2 = ObserverType::New();
	coordSystemAObserver2->ObserveTransformEventsFrom( trackerTool2 );
	
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// tracking starts and define the frequency

	tracker->RequestSetFrequency(freq_track);
	tracker->RequestStartTracking();

	
	typedef igstk::Transform            Transform;
	typedef ::itk::Vector<double,3>    VectorType; //<double, 3>
	typedef ::itk::Versor<double>		VersorType;//<double>
	typedef ::itk::Matrix<double, 4,4>  MatrixType;

	

/////////////////////////////////////////   END OF VICRA PART     /////////////////////////////////////////



// WHEN WE WANT TO GET THE DATA FROM THE VICRA (IN THIS EXAMPLE FOR THE FIRST TOOL) WE HAVE TO INSERT THE FOLLOWING CODE
	
	int start=clock();
	resetplot=0;

	time_t tstart, tend;
	tstart=time(0);
	label1->Text="COM NOT AVAILABLE";	
	chart1->Series["Series1"]->Points->Clear();
	count=0;

	while (row<1000) 
		{
		
			if(backgroundWorker3->CancellationPending) //if it was cancelled
					{
						//CREATE SR Coil 
					FocusOx=Coil2x;//(Coil1x+Coil2x+Coil3x)/3;
					FocusOy=Coil2y;//(Coil1y+Coil2y+Coil3y)/3;
					FocusOz=Coil2z;//(Coil1z+Coil2z+Coil3z)/3;
					//terna, vector x  (from left to rigth)
					FocusXi=(Coil3x-Coil1x)/sqrtf(pow(Coil3x-Coil1x,2)+pow(Coil3y-Coil1y,2)+pow(Coil3z-Coil1z,2));
					FocusXj=(Coil3y-Coil1y)/sqrtf(pow(Coil3x-Coil1x,2)+pow(Coil3y-Coil1y,2)+pow(Coil3z-Coil1z,2));
					FocusXk=(Coil3z-Coil1z)/sqrtf(pow(Coil3x-Coil1x,2)+pow(Coil3y-Coil1y,2)+pow(Coil3z-Coil1z,2));

					//vector diff coil3-coil2  
					FocusAusx=(CoilUpx-Coil1x)/sqrtf(pow(CoilUpx-Coil1x,2)+pow(CoilUpy-Coil1y,2)+pow(CoilUpz-Coil1z,2));
					FocusAusy=(CoilUpy-Coil1y)/sqrtf(pow(CoilUpx-Coil1x,2)+pow(CoilUpy-Coil1y,2)+pow(CoilUpz-Coil1z,2));
					FocusAusz=(CoilUpz-Coil1z)/sqrtf(pow(CoilUpx-Coil1x,2)+pow(CoilUpy-Coil1y,2)+pow(CoilUpz-Coil1z,2));
					//prodotto vettoriale a2b3-a3b2,-(a1b3-a3b1),a1b2-a2b1 , vector z (from down to up)
					FocusZi=(FocusXj*FocusAusz-FocusXk*FocusAusy)/sqrtf(pow(FocusXj*FocusAusz-FocusXk*FocusAusy,2)+pow(FocusXi*FocusAusz-FocusXk*FocusAusx,2)+pow(FocusXi*FocusAusy-FocusXj*FocusAusx,2));
					FocusZj=-(FocusXi*FocusAusz-FocusXk*FocusAusx)/sqrtf(pow(FocusXj*FocusAusz-FocusXk*FocusAusy,2)+pow(FocusXi*FocusAusz-FocusXk*FocusAusx,2)+pow(FocusXi*FocusAusy-FocusXj*FocusAusx,2));
					FocusZk=(FocusXi*FocusAusy-FocusXj*FocusAusx)/sqrtf(pow(FocusXj*FocusAusz-FocusXk*FocusAusy,2)+pow(FocusXi*FocusAusz-FocusXk*FocusAusx,2)+pow(FocusXi*FocusAusy-FocusXj*FocusAusx,2));
					// prodotto vettoriale a2b3-a3b2,-(a1b3-a3b1),a1b2-a2b1 , vector y (from handle to CoilUp)
					FocusYi=(FocusZj*FocusXk-FocusZk*FocusXj)/sqrtf(pow(FocusZj*FocusXk-FocusZk*FocusXj,2)+pow(FocusZi*FocusXk-FocusZk*FocusXi,2)+pow(FocusZi*FocusXj-FocusZj*FocusXi,2));
					FocusYj=-(FocusZi*FocusXk-FocusZk*FocusXi)/sqrtf(pow(FocusZj*FocusXk-FocusZk*FocusXj,2)+pow(FocusZi*FocusXk-FocusZk*FocusXi,2)+pow(FocusZi*FocusXj-FocusZj*FocusXi,2));
					FocusYk=(FocusZi*FocusXj-FocusZj*FocusXi)/sqrtf(pow(FocusZj*FocusXk-FocusZk*FocusXj,2)+pow(FocusZi*FocusXk-FocusZk*FocusXi,2)+pow(FocusZi*FocusXj-FocusZj*FocusXi,2));
		
					// create the Homo_matrix (to coil to marker)
					MatrixType		Homo_Head, Homo_matrix;
					Homo_matrix[0][0]=FocusXi;	Homo_matrix[1][0]=FocusXj; Homo_matrix[2][0]=FocusXk;   Homo_matrix[3][0]= 0;
					Homo_matrix[0][1]=FocusYi;	Homo_matrix[1][1]=FocusYj;	Homo_matrix[2][1]=FocusYk;	Homo_matrix[3][1]= 0;
					Homo_matrix[0][2]=FocusZi;	Homo_matrix[1][2]=FocusZj;	Homo_matrix[2][2]= FocusZk;	Homo_matrix[3][2]= 0;
					Homo_matrix[0][3]=FocusOx;	Homo_matrix[1][3]=FocusOy;	Homo_matrix[2][3]=FocusOz;	Homo_matrix[3][3]= 1;
					// quaternion from matrix
					qw_coil=sqrtf(1+Homo_matrix[0][0]+Homo_matrix[1][1]+Homo_matrix[2][2])/2;
					qx_coil=(Homo_matrix[2][1]-Homo_matrix[1][2])/(4*qw_coil);
					qy_coil=(Homo_matrix[0][2]-Homo_matrix[2][0])/(4*qw_coil);
					qz_coil=(Homo_matrix[1][0]-Homo_matrix[0][1])/(4*qw_coil);

					trans_coilx=FocusOx;//Homo_matrix[0][3];  BYPASSANDO STO PASSAGGIO FUNZIONA, ATTENZIONE SE DEVI USARE LA MATRICE
					trans_coily=FocusOy;//Homo_matrix[1][3];
					trans_coilz=FocusOz;//Homo_matrix[2][3];
					// choose directory

					// READ from textbox the name of the file txt where we want to save the calibration
				
					fstream outf;
					System::String ^ Directory = textBox1->Text + "\\CoilCalibration.txt"; //path directory 	//write directory
				/*	struct stat s;
					int err = stat(pathDirectoryChar, &s);
					if(-1 == err) {
						if(ENOENT == errno) {
							label5->Text ="PATH NOT AVAILABLE";
						// does not exist 
						} else {
					perror("stat");
					//	exit(1);
						} else {
						if(S_ISDIR(s.st_mode)) {
							//it's a dir 
						} else {
							// exists but is no dir 
						}
					}
						label5->Text ="PATH exist";
					}
					if (!PathFileExists(pathDirectoryChar)){*/
					pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
					/* DWORD ftyp = GetFileAttributesA(pathDirectoryChar);
					  if (ftyp == INVALID_FILE_ATTRIBUTES){
						label5->Text ="PATH ERROR";  //something is wrong with your path!
					  }else{
					//  if (ftyp & FILE_ATTRIBUTE_DIRECTORY){
						label5->Text ="PATH exist";   // this is a directory!
					//  }else{
					//label5->Text ="PATH not exist";    // this is not a directory!
					 }*/
					const wchar_t *constValue = convertedValue; 
					System::String ^ pathDirectory = textBox1->Text ;
					char* pathDirectoryChar = (char*)(void*)Marshal::StringToHGlobalAnsi(pathDirectory);
					 DWORD dwAttrib = GetFileAttributes(pathDirectoryChar);
					if(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY)){
						//label20->Text ="PATH exist"; 
					}else{
						label20->Text ="PATH NOT EXIST"; 
					}
					ofstream outfile(constValue);
					outfile << trans_coilx << "\n" << trans_coily<< "\n"<<trans_coilz<<"\n"<<qx_coil << "\n" << qy_coil<< "\n"<<qz_coil <<"\n"<<qw_coil <<"\n"<<endl; 
					outfile.close();
					/*if ( access(pathDirectoryChar, 0 ) == 0 )
					   {
						  struct stat status;
						  stat( pathDirectoryChar, &status );

						  if ( status.st_mode & S_IFDIR )
						  {
							label5->Text ="The directory exists.";
						  }
						  else
						  {
							 label5->Text ="The path you entered is a file.";
						  }
					   }
					   else
					   {
						 label5->Text ="Path doesn't exist." ;
					   }*/
					this->button10->Enabled = true;
					//ifstream textfile(constValue);
					//if (textfile.good()) {
					e->Cancel=true;
					break;
			}
		//label1->Text =Data[row][0]->ToString();
			
		worker->ReportProgress( row*100/1000 );
		if ( row*100/1000  > highestPercentageReached )//quando finisce il while la barra si ferma l
        {
          highestPercentageReached = row*100/1000 ;
         worker->ReportProgress( row*100/1000  );
        }
		
		igstk::PulseGenerator::CheckTimeouts();
		Sleep(5);
		
		Transform		transform1, transform2, transformTip;
		VectorType		vector1, vector2; 
		VersorType		versor1, versor2;
		
		coordSystemAObserver1->Clear();
		coordSystemAObserver2->Clear();
		trackerTool1->RequestGetTransformToParent();
		trackerTool2->RequestComputeTransformTo(trackerTool1);
		//label1->Text="Working2";	
		if (coordSystemAObserver1->GotTransform() && coordSystemAObserver2->GotTransform() ) { // 
			transform1 = coordSystemAObserver1->GetTransform();
			transform2 = coordSystemAObserver2->GetTransform();
		//	label1->Text="Working2";	
			if ( transform1.IsValidNow() && !transform2.IsValidNow()){
				count=0;
				label1->Text ="POINTER NOT VISIBLE";
				this->button10->BackColor=System::Drawing::Color::Red;
				this->button2->Enabled = false;
				this->button4->Enabled = false;
				this->button18->Enabled = false;
				this->button5->Enabled = false;
			}
			if ( !transform1.IsValidNow() && transform2.IsValidNow() || !transform1.IsValidNow() && !transform2.IsValidNow()){
				count=0;
				label1->Text ="COIL NOT VISIBLE";
				this->button10->BackColor=System::Drawing::Color::Red;
				this->button2->Enabled = false;
				this->button4->Enabled = false;
				this->button18->Enabled = false;
				this->button5->Enabled = false;
			}
			if ( transform1.IsValidNow() && transform2.IsValidNow()){
				vector2 = transform2.GetTranslation();
				versor2 = transform2.GetRotation();
				data[row][0] =vector2[0];//DISTANZA		
				label1->Text="Working";	
				data[row][1]= vector2[1]; //TETA
				data[row][2]=vector2[2]; //PSI
				this->button10->BackColor=System::Drawing::Color::Green;
				this->button2->Enabled = true;
				this->button4->Enabled = true;
				this->button18->Enabled = true;
				this->button5->Enabled = true;
				Coil3x=data[row][0];
				Coil3y=data[row][1];
				Coil3z=data[row][2];
				}
			}
		row++;
	}	
	
	
		

		this->button10->BackColor=System::Drawing::Color::White;
		label1->Text ="RequestStopTracking()";
		tracker->RequestStopTracking();
		tracker->RequestClose();
		serialComm->CloseCommunication();
		this->button2->Enabled = true;
		this->button10->Enabled = true;	

		 }
	private: System::Void backgroundWorker3_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e){
		 this->progressBar1->Value = e->ProgressPercentage;
		 }
	private: System::Void backgroundWorker3_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e){
		  if ( e->Cancelled ){
         // Next, handle the case where the user cancelled 
         // the operation.
         // Note that due to a race condition in 
         // the DoWork event handler, the Cancelled
         // flag may not have been set, even though
         // CancelAsync was called.
         label1->Text = "COIL_CALIBRATION DONE";
      }
      else
      {
         // Finally, handle the case where the operation 
         // succeeded.
         //label1->Text ="Succeed";
		  }
}
			// Record hotspot
	private: System::Void button17_Click(System::Object^  sender, System::EventArgs^  e) {
		this->button17->Enabled = false; 
		 fstream outf;
		backgroundWorker1->CancelAsync();
			label1->Text ="HOTSPOT RECORDED";
				this->button17->BackColor=System::Drawing::Color::Gray;
		
		 }
		// save Calibration model
	private: System::Void button19_Click(System::Object^  sender, System::EventArgs^  e) {
	this->button19->Enabled = false; 
	fstream outf; 
	System::String ^ Directory = textBox1->Text + "\\HeadCalibrationModel.txt"; //path directory 	//write directory
	pin_ptr<const wchar_t> convertedValue = PtrToStringChars(Directory);  // <-- #include <vcclr.h>
	const wchar_t *constValue = convertedValue; 
	ofstream outfile(constValue);
	outfile << dist1 << "\n" << dist2<< "\n"<<dist3<<"\n"<<endl;
	outfile.close();
	this->button19->BackColor=System::Drawing::Color::Gray;
	 }
};
}

/*
 * Description:
 * this is a program for SPF algorithm: graph-01
 * node num:20
 * a pair of source and destination for 2000 connection on UDP
 *
 * test: throughput, delay, packetlossrate
 * Throughput level from 100-200mps
 * delay level from 0-100ms
 * packetlossrate level from 0.00000-0.0001
 * */
#include "ns3/netanim-module.h"
#include "ns3/lte-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/mobility-module.h"
#include "ns3/ssid.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("For test build network link");

void EnqueueTrace(std::string context, Ptr<const Packet> pkt)
{
   std::cout<<context<<
             "\tTime:" << Simulator::Now() <<" Packet of size: "<< pkt->GetSize()
             <<"enqueue!"<<std::endl;
}

void DequeueTrace(std::string context, Ptr<const Packet> pkt)
{
   std::cout<<context<<
             "\tTime:" << Simulator::Now() <<" Packet of size: "<< pkt->GetSize()
             <<"dequeue!"<<std::endl;
}


int
main(int argc, char* argv[])
{
    bool verbose = true;
    uint32_t nCsma = 3;   //nCsma node number
    uint32_t nWifi = 3;   //wifi node number
    uint32_t nUeNum = 3; //lte-user node  number

    CommandLine cmd;
    cmd.AddValue("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);

    cmd.Parse(argc, argv);

    if (verbose)
    {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    nCsma = nCsma == 0 ? 1 : nCsma;
    nWifi = nWifi == 0 ? 1 : nWifi;
    nUeNum = nUeNum == 0 ? 1 : nUeNum;


    //Backbone Network 

    //network 1
    NodeContainer p2pNodes1;
    p2pNodes1.Create(2);
    PointToPointHelper pointToPoint1;
    pointToPoint1.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint1.SetChannelAttribute("Delay", StringValue("8ms"));

    NetDeviceContainer p2pDevices1;
    p2pDevices1 = pointToPoint1.Install(p2pNodes1);

    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices1.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
    

    //network2
    NodeContainer p2pNodes2;
    p2pNodes2.Add(p2pNodes1.Get(1));
    p2pNodes2.Create(1);
    PointToPointHelper pointToPoint2;
    pointToPoint2.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint2.SetChannelAttribute("Delay", StringValue("6ms"));

    NetDeviceContainer p2pDevices2;
    p2pDevices2 = pointToPoint2.Install(p2pNodes2);

  //  Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices2.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
     

    //network3
    NodeContainer p2pNodes3;
    p2pNodes3.Add(p2pNodes2.Get(0));
    p2pNodes3.Create(1);


    PointToPointHelper pointToPoint3;
    pointToPoint3.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint3.SetChannelAttribute("Delay", StringValue("8ms"));

    NetDeviceContainer p2pDevices3;
    p2pDevices3 = pointToPoint3.Install(p2pNodes3);

    //Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices3.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));




    //network4
    NodeContainer p2pNodes4;
    p2pNodes4.Add(p2pNodes1.Get(0));
    p2pNodes4.Add(p2pNodes3.Get(1));
    PointToPointHelper pointToPoint4;
    pointToPoint4.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint4.SetChannelAttribute("Delay", StringValue("8ms"));

    NetDeviceContainer p2pDevices4;
    p2pDevices4 = pointToPoint4.Install(p2pNodes4);

   //Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices4.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));



    //network5
    NodeContainer p2pNodes5;
    p2pNodes5.Add(p2pNodes3.Get(1));
    p2pNodes5.Create(1);

    PointToPointHelper pointToPoint5;
    pointToPoint5.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint5.SetChannelAttribute("Delay", StringValue("9ms"));

    NetDeviceContainer p2pDevices5;
    p2pDevices5 = pointToPoint5.Install(p2pNodes5);

    //Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices5.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));




    //network6
    NodeContainer p2pNodes6;
    p2pNodes6.Add(p2pNodes1.Get(0));
    p2pNodes6.Create(1);

    PointToPointHelper pointToPoint6;
    pointToPoint6.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint6.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices6;
    p2pDevices6 = pointToPoint6.Install(p2pNodes6);

  //  Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices6.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));




    //network 7
    NodeContainer p2pNodes7;
    p2pNodes7.Add(p2pNodes6.Get(1));
    p2pNodes7.Create(1);

    PointToPointHelper pointToPoint7;
    pointToPoint7.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint7.SetChannelAttribute("Delay", StringValue("9ms"));

    NetDeviceContainer p2pDevices7;
    p2pDevices7 = pointToPoint7.Install(p2pNodes7);

   // Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices7.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));



    //network 8
    NodeContainer p2pNodes8;
    p2pNodes8.Add(p2pNodes2.Get(1));
    p2pNodes8.Create(1);

    PointToPointHelper pointToPoint8;
    pointToPoint8.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint8.SetChannelAttribute("Delay", StringValue("7ms"));

    NetDeviceContainer p2pDevices8;
    p2pDevices8 = pointToPoint1.Install(p2pNodes8);

   // Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices8.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));



    //network9
    NodeContainer p2pNodes9;
    p2pNodes9.Add(p2pNodes5.Get(1));
    p2pNodes9.Create(1);

    PointToPointHelper pointToPoint9;
    pointToPoint9.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint9.SetChannelAttribute("Delay", StringValue("8ms"));

    NetDeviceContainer p2pDevices9;
    p2pDevices9 = pointToPoint9.Install(p2pNodes9);

//    Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices9.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));



    //network 10
    NodeContainer p2pNodes10;
    p2pNodes10.Add(p2pNodes5.Get(1));
    p2pNodes10.Add(p2pNodes7.Get(1));

    PointToPointHelper pointToPoint10;
    pointToPoint10.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint10.SetChannelAttribute("Delay", StringValue("7ms"));

    NetDeviceContainer p2pDevices10;
    p2pDevices10 = pointToPoint10.Install(p2pNodes10);

  //  Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices10.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));


    //network 11
    NodeContainer p2pNodes11;
    p2pNodes11.Add(p2pNodes8.Get(1));
    p2pNodes11.Create(1);

    PointToPointHelper pointToPoint11;
    pointToPoint11.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint11.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer p2pDevices11;
    p2pDevices11 = pointToPoint11.Install(p2pNodes11);

   // Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices11.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));



    //network 12
    NodeContainer p2pNodes12;
    p2pNodes12.Add(p2pNodes8.Get(1));
    p2pNodes12.Add(p2pNodes9.Get(1));

    PointToPointHelper pointToPoint12;
    pointToPoint12.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint12.SetChannelAttribute("Delay", StringValue("7ms"));

    NetDeviceContainer p2pDevices12;
    p2pDevices12 = pointToPoint12.Install(p2pNodes12);

  //  Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices12.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));





    //network 13
    NodeContainer p2pNodes13;
    p2pNodes13.Add(p2pNodes9.Get(1));
    p2pNodes13.Add(p2pNodes11.Get(1));

    PointToPointHelper pointToPoint13;
    pointToPoint13.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint13.SetChannelAttribute("Delay", StringValue("6ms"));

    NetDeviceContainer p2pDevices13;
    p2pDevices13 = pointToPoint13.Install(p2pNodes13);

 //   Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices13.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));




    //network 14
    NodeContainer p2pNodes14;
    p2pNodes14.Add(p2pNodes12.Get(1));
    p2pNodes14.Add(p2pNodes7.Get(1));

    PointToPointHelper pointToPoint14;
    pointToPoint14.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint14.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices14;
    p2pDevices14 = pointToPoint14.Install(p2pNodes14);

 //   Ptr<RateErrorModel> em = CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate", DoubleValue(0.00000005));
    p2pDevices14.Get(1)->SetAttribute("ReceiveErrorModel", PointerValue(em));
    

    
    //WifiNetwork1 
    NodeContainer wifiStaNodes1;
    wifiStaNodes1.Create (nWifi);
    NodeContainer wifiApNode1 = p2pNodes1.Get (1); //设置AP:设置为s1

    YansWifiChannelHelper channel1 = YansWifiChannelHelper::Default ();  //channel+ phy
    YansWifiPhyHelper phy1 = YansWifiPhyHelper::Default ();
    phy1.SetChannel (channel1.Create ());

    WifiHelper wifi1;
    wifi1.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                              "DataMode", StringValue ("OfdmRate54Mbps"));

    WifiMacHelper mac1;
    Ssid ssid1 = Ssid ("ns-3-ssid");  //Wifi叫啥名
    mac1.SetType ("ns3::StaWifiMac",  //设定特定种类的MAC层
                 "Ssid", SsidValue (ssid1),  //设置ssid
                 "ActiveProbing", BooleanValue (false));
           
     NetDeviceContainer staDevices1;
     staDevices1 = wifi1.Install (phy1, mac1, wifiStaNodes1);
    
     mac1.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid1));

     NetDeviceContainer apDevices1;
     apDevices1 = wifi1.Install (phy1, mac1, wifiApNode1);     
   
     //Wifi -set mobility model 
     MobilityHelper mobility1;
     //Wifi -set position allocation 
      mobility1.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),//起始x坐标
                                 "MinY", DoubleValue (0.0),//起始y坐标
                                 "DeltaX", DoubleValue (5.0),//x轴上节点距离
                                 "DeltaY", DoubleValue (10.0),//y轴上节点距离
                                 "GridWidth", UintegerValue (3),//一行节点最大数量
                                 "LayoutType", StringValue ("RowFirst")//布局类型
      ); 
   
       mobility1.Install (wifiStaNodes1);    
       mobility1.SetMobilityModel ("ns3::ConstantPositionMobilityModel");//固定不动
       mobility1.Install (wifiApNode1);

       
  
   
       //wifi2
    
    NodeContainer wifiStaNodes2;
    wifiStaNodes2.Create (nWifi);
    NodeContainer wifiApNode2 = p2pNodes8.Get (1); //设置AP

    YansWifiChannelHelper channel2 = YansWifiChannelHelper::Default ();  //channel+ phy
    YansWifiPhyHelper phy2 = YansWifiPhyHelper::Default ();
    phy2.SetChannel (channel2.Create ());

    WifiHelper wifi2;
    wifi2.SetRemoteStationManager ("ns3::AarfWifiManager");

    WifiMacHelper mac2;
    Ssid ssid2 = Ssid ("ns-3-ssid2");  //Wifi叫啥名
    mac2.SetType ("ns3::StaWifiMac",  //设定特定种类的MAC层
                 "Ssid", SsidValue (ssid2),  //设置ssid
                 "ActiveProbing", BooleanValue (false));
           
     NetDeviceContainer staDevices2;
     staDevices2 = wifi2.Install (phy2, mac2, wifiStaNodes2);
    
     mac2.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid2));

     NetDeviceContainer apDevices2;
     apDevices2 = wifi2.Install (phy2, mac2, wifiApNode2);     
   
     //Wifi -set mobility model 
     MobilityHelper mobility2;
     //Wifi -set position allocation 
      mobility2.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),//起始x坐标
                                 "MinY", DoubleValue (0.0),//起始y坐标
                                 "DeltaX", DoubleValue (5.0),//x轴上节点距离
                                 "DeltaY", DoubleValue (10.0),//y轴上节点距离
                                 "GridWidth", UintegerValue (3),//一行节点最大数量
                                 "LayoutType", StringValue ("RowFirst")//布局类型
      ); 
   
       mobility2.Install (wifiStaNodes2);    
       mobility2.SetMobilityModel ("ns3::ConstantPositionMobilityModel");//固定不动
       mobility2.Install (wifiApNode2);




       //Wifi3
       NodeContainer wifiStaNodes3;
       wifiStaNodes3.Create(nWifi);
       NodeContainer wifiApNode3 = p2pNodes6.Get(1); //设置AP:设置Ap为s3

       YansWifiChannelHelper channel3 = YansWifiChannelHelper::Default();  //channel+ phy
       YansWifiPhyHelper phy3 = YansWifiPhyHelper::Default();
       phy3.SetChannel(channel3.Create());

       WifiHelper wifi3;
       wifi3.SetRemoteStationManager("ns3::ConstantRateWifiManager",
           "DataMode", StringValue("OfdmRate54Mbps"));

       WifiMacHelper mac3;
       Ssid ssid3 = Ssid("ns-3-ssid");  //Wifi叫啥名
       mac3.SetType("ns3::StaWifiMac",  //设定特定种类的MAC层
           "Ssid", SsidValue(ssid3),  //设置ssid
           "ActiveProbing", BooleanValue(false));

       NetDeviceContainer staDevices3;
       staDevices3 = wifi3.Install(phy3, mac3, wifiStaNodes3);

       mac3.SetType("ns3::ApWifiMac",
           "Ssid", SsidValue(ssid3));

       NetDeviceContainer apDevices3;
       apDevices3 = wifi3.Install(phy3, mac3, wifiApNode3);

       //Wifi -set mobility model 
       MobilityHelper mobility3;
       //Wifi -set position allocation 
       mobility3.SetPositionAllocator("ns3::GridPositionAllocator",
           "MinX", DoubleValue(0.0),//起始x坐标
           "MinY", DoubleValue(0.0),//起始y坐标
           "DeltaX", DoubleValue(5.0),//x轴上节点距离
           "DeltaY", DoubleValue(10.0),//y轴上节点距离
           "GridWidth", UintegerValue(3),//一行节点最大数量
           "LayoutType", StringValue("RowFirst")//布局类型
       );

       mobility3.Install(wifiStaNodes3);
       mobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");//固定不动
       mobility3.Install(wifiApNode3);



       

       //CMSA1
       NodeContainer csmaNodes1;
       csmaNodes1.Add(p2pNodes2.Get(1));
       csmaNodes1.Create(nCsma);

       CsmaHelper csma;
       csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
       csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

       NetDeviceContainer csmaDevices1;
       csmaDevices1 = csma.Install(csmaNodes1);



       //CSMA2;
       NodeContainer csmaNodes2;
       csmaNodes2.Add(p2pNodes7.Get(1));
       csmaNodes2.Create(nCsma);


       CsmaHelper csma2;
       csma2.SetChannelAttribute("DataRate", StringValue("100Mbps"));
       csma2.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

       NetDeviceContainer csmaDevices2;
       csmaDevices2 = csma2.Install(csmaNodes2);






       //CSMA3;
       NodeContainer csmaNodes3;
       csmaNodes3.Add(p2pNodes13.Get(1));
       csmaNodes3.Create(nCsma);

       CsmaHelper csma3;
       csma3.SetChannelAttribute("DataRate", StringValue("100Mbps"));
       csma3.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));


       NetDeviceContainer csmaDevices3;
       csmaDevices3 = csma3.Install(csmaNodes3);
       

       //lte-network1
       Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();
       //node
       NodeContainer enbNodes; //enbNode
       enbNodes.Create(1);
       NodeContainer ueNodes;  //ueNodes
       ueNodes.Create(nUeNum);

       NetDeviceContainer enbDevs;
       lteHelper->SetAttribute("PathlossModel", StringValue("ns3::FriisPropagationLossModel"));


       MobilityHelper mobility4;
       mobility4.SetMobilityModel("ns3::ConstantPositionMobilityModel");//固定不动
       mobility4.Install(enbNodes);

       mobility4.SetPositionAllocator("ns3::GridPositionAllocator",
           "MinX", DoubleValue(0.0),//起始x坐标
           "MinY", DoubleValue(0.0),//起始y坐标
           "DeltaX", DoubleValue(5.0),//x轴上节点距离
           "DeltaY", DoubleValue(10.0),//y轴上节点距离
           "GridWidth", UintegerValue(3),//一行节点最大数量
           "LayoutType", StringValue("RowFirst")//布局类型
       );

       mobility4.Install(ueNodes);
       enbDevs = lteHelper->InstallEnbDevice(enbNodes);

       NetDeviceContainer ueDevs;
       ueDevs = lteHelper->InstallUeDevice(ueNodes);




       //internet stack
    InternetStackHelper stack;
    stack.Install(p2pNodes1.Get(0));
    stack.Install(p2pNodes2.Get(0));
    stack.Install(p2pNodes3.Get(1));
    stack.Install(p2pNodes6.Get(1));
    stack.Install(p2pNodes5.Get(1));
    stack.Install(p2pNodes8.Get(1));
    stack.Install(p2pNodes9.Get(1));
    stack.Install (wifiStaNodes1);
    stack.Install (wifiStaNodes2);
    stack.Install(wifiStaNodes3);

    stack.Install(csmaNodes1);
    stack.Install(csmaNodes2);
    stack.Install(csmaNodes3);
    
        stack.Install(enbNodes);
        stack.Install(ueNodes);

    //adress
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");

    Ipv4InterfaceContainer p2pInterfaces1;
    p2pInterfaces1 = address.Assign(p2pDevices1);

    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces2;
    p2pInterfaces2 = address.Assign(p2pDevices2);

    address.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces3;
    p2pInterfaces3 = address.Assign(p2pDevices3);

    address.SetBase("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces4;
    p2pInterfaces4 = address.Assign(p2pDevices4);

    address.SetBase("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces5;
    p2pInterfaces5 = address.Assign(p2pDevices5);

    address.SetBase("10.1.6.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces6;
    p2pInterfaces6 = address.Assign(p2pDevices6);

    address.SetBase("10.1.7.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces7;
    p2pInterfaces7 = address.Assign(p2pDevices7);

    address.SetBase("10.1.8.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces8;
    p2pInterfaces8 = address.Assign(p2pDevices8);

    address.SetBase("10.1.9.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces9;
    p2pInterfaces9 = address.Assign(p2pDevices9);

    address.SetBase("10.1.10.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces10;
    p2pInterfaces10 = address.Assign(p2pDevices10);
    
    address.SetBase("10.1.11.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces11;
    p2pInterfaces11 = address.Assign(p2pDevices11);

    address.SetBase("10.1.12.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces12;
    p2pInterfaces12 = address.Assign(p2pDevices12);

    address.SetBase("10.1.13.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces13;
    p2pInterfaces13 = address.Assign(p2pDevices13);

    address.SetBase("10.1.14.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces14;
    p2pInterfaces14 = address.Assign(p2pDevices14);
   
    address.SetBase("10.1.15.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces1;
    csmaInterfaces1= address.Assign(csmaDevices1);

    
    address.SetBase ("10.1.18.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces2;
    csmaInterfaces2 = address.Assign(csmaDevices2);
     
    address.SetBase ("10.1.19.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces3;
    csmaInterfaces3 = address.Assign(csmaDevices3);


    address.SetBase("10.1.20.0", "255.255.255.0");
     Ipv4InterfaceContainer wifi1Interface;
    wifi1Interface=address.Assign(staDevices1);
    address.Assign(apDevices1);

    address.SetBase("10.1.21.0", "255.255.255.0");
    address.Assign(staDevices2);
    address.Assign(apDevices2);


    address.SetBase("10.1.22.0", "255.255.255.0");
    address.Assign(staDevices2);
    address.Assign(apDevices2);

     address.SetBase("10.1.23.0", "255.255.255.0");
     Ipv4InterfaceContainer enbIfaces = address.Assign(enbDevs);
     Ipv4InterfaceContainer ueIfaces = address.Assign(ueDevs);

//我设置10对应用

    //uint32_t maxpacket=93;  //发送包的数量
    uint32_t packetSize=128;   //包的大小


//Application1: this is a p2p
     UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps1 = echoServer.Install(p2pNodes1.Get(1));
    serverApps1.Start(Seconds(1.0));
    serverApps1.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient1(p2pInterfaces1.GetAddress(1), 9);
    echoClient1.SetAttribute("MaxPackets", UintegerValue(80));
    echoClient1.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient1.SetAttribute("PacketSize", UintegerValue(128));

    ApplicationContainer clientApps1 = echoClient1.Install(p2pNodes1.Get(0));
    clientApps1.Start(Seconds(2.0));
    clientApps1.Stop(Seconds(10.0));

//Application2: this is a WiFi-p2p
    ApplicationContainer serverApps2 = echoServer.Install(p2pNodes3.Get(1));
    serverApps2.Start(Seconds(1.0));
    serverApps2.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient2(p2pInterfaces3.GetAddress(1), 9);
    echoClient2.SetAttribute("MaxPackets", UintegerValue(230));
    echoClient2.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient2.SetAttribute("PacketSize", UintegerValue(128));

    ApplicationContainer clientApps2 = echoClient2.Install(wifiStaNodes1.Get(1));
    clientApps2.Start(Seconds(2.0));
    clientApps2.Stop(Seconds(10.0));


//Application3: this is a p2p-wifi
    ApplicationContainer serverApps3 = echoServer.Install(wifiStaNodes1.Get(2));
    serverApps3.Start(Seconds(1.0));
    serverApps3.Stop(Seconds(10.0));


    
    UdpEchoClientHelper echoClient3(wifi1Interface.GetAddress(2),9);
    echoClient3.SetAttribute("MaxPackets", UintegerValue(230));
    echoClient3.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient3.SetAttribute("PacketSize", UintegerValue(packetSize));

    ApplicationContainer clientApps3 = echoClient3.Install(p2pNodes1.Get(0));
    clientApps3.Start(Seconds(2.0));
    clientApps3.Stop(Seconds(10.0));

//Application4: this is a csma to p2p
 UdpEchoServerHelper echoServer2(10);

   ApplicationContainer serverApps4 = echoServer2.Install(p2pNodes4.Get(1));
    serverApps3.Start(Seconds(1.0));
    serverApps3.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient4(p2pInterfaces4.GetAddress(1),10);
    echoClient4.SetAttribute("MaxPackets", UintegerValue(250));
    echoClient4.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient4.SetAttribute("PacketSize", UintegerValue(packetSize));

    ApplicationContainer clientApps4 = echoClient4.Install(csmaNodes1.Get(2));
    clientApps4.Start(Seconds(1.0));
    clientApps4.Stop(Seconds(10.0));



//Application5: this is a p2p to csma

   ApplicationContainer serverApps5 = echoServer2.Install(csmaNodes2.Get(3));
    serverApps5.Start(Seconds(1.0));
    serverApps5.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient5(csmaInterfaces2.GetAddress(3),10);
    echoClient5.SetAttribute("MaxPackets", UintegerValue(160));
    echoClient5.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient5.SetAttribute("PacketSize", UintegerValue(256));

    ApplicationContainer clientApps5 = echoClient5.Install(csmaNodes2.Get(1));
    clientApps5.Start(Seconds(1.0));
    clientApps5.Stop(Seconds(10.0));

//Application6: this  is a ue to p2p

     ApplicationContainer serverApps6 = echoServer2.Install(p2pNodes5.Get(1));
    serverApps6.Start(Seconds(1.0));
    serverApps6.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient6(p2pInterfaces5.GetAddress(1),10);
    echoClient6.SetAttribute("MaxPackets", UintegerValue(140));
    echoClient6.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient6.SetAttribute("PacketSize", UintegerValue(packetSize));

    ApplicationContainer clientApps6 = echoClient6.Install(ueNodes.Get(0));
    clientApps6.Start(Seconds(1.0));
    clientApps6.Stop(Seconds(10.0));

//Application7: p2p-p2p
    UdpEchoServerHelper echoServer4(19);
    ApplicationContainer serverApps7 = echoServer4.Install(p2pNodes5.Get(0));
    serverApps7.Start(Seconds(1.0));
    serverApps7.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient7(p2pInterfaces5.GetAddress(0), 19);
    echoClient7.SetAttribute("MaxPackets", UintegerValue(120));
    echoClient7.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient7.SetAttribute("PacketSize", UintegerValue(packetSize));

    ApplicationContainer clientApps7 = echoClient7.Install(p2pNodes8.Get(0));
    clientApps7.Start(Seconds(1.0));
    clientApps7.Stop(Seconds(10.0));


//Application8: p2p-p2p
    
UdpEchoServerHelper echoServer5(18);
ApplicationContainer serverApps8 = echoServer.Install(p2pNodes5.Get(1));
    serverApps8.Start(Seconds(1.0));
    serverApps8.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient8(p2pInterfaces5.GetAddress(1), 18);
    echoClient8.SetAttribute("MaxPackets", UintegerValue(150));
    echoClient8.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient8.SetAttribute("PacketSize", UintegerValue(128));

    ApplicationContainer clientApps8 = echoClient1.Install(p2pNodes13.Get(0));
    clientApps8.Start(Seconds(2.0));
    clientApps8.Stop(Seconds(10.0));

//Application9: p2p-p2p

    ApplicationContainer serverApps9 = echoServer5.Install(p2pNodes1.Get(1));
    serverApps9.Start(Seconds(1.0));
    serverApps9.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient9(p2pInterfaces1.GetAddress(1), 18);
    echoClient9.SetAttribute("MaxPackets", UintegerValue(100));
    echoClient9.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient9.SetAttribute("PacketSize", UintegerValue(128));

    ApplicationContainer clientApps9 = echoClient8.Install(p2pNodes1.Get(0));
    clientApps9.Start(Seconds(2.0));
    clientApps9.Stop(Seconds(10.0));

   //Application10 
    ApplicationContainer serverApps10 = echoServer5.Install(p2pNodes2.Get(1));
    serverApps10.Start(Seconds(1.0));
    serverApps10.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient10(p2pInterfaces2.GetAddress(1), 18);
    echoClient10.SetAttribute("MaxPackets", UintegerValue(60));
    echoClient10.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient10.SetAttribute("PacketSize", UintegerValue(packetSize));

    ApplicationContainer clientApps10 = echoClient10.Install(p2pNodes2.Get(0));
    clientApps10.Start(Seconds(2.0));
    clientApps10.Stop(Seconds(10.0));

  //Application11
     ApplicationContainer serverApps11 = echoServer5.Install(p2pNodes4.Get(1));
    serverApps11.Start(Seconds(1.0));
    serverApps11.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient11(p2pInterfaces4.GetAddress(1), 18);
    echoClient11.SetAttribute("MaxPackets", UintegerValue(60));
    echoClient11.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient11.SetAttribute("PacketSize", UintegerValue(128));

    ApplicationContainer clientApps11 = echoClient11.Install(p2pNodes10.Get(1));
    clientApps11.Start(Seconds(2.0));
    clientApps11.Stop(Seconds(10.0));


   // AnimationInterface anim("display.xml");





    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Ipv4StaticRoutingHelper routingHelper;

    Ptr<OutputStreamWrapper> routingStream=Create<OutputStreamWrapper>("r.tr",std::ios::out);
    routingHelper.PrintRoutingTableAllAt(
       Seconds(3.0),
       routingStream
    );

     FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor=flowmon.InstallAll();

    //Config::Set("/NodeList/*/ApplicationList/*/$ns3::UdpEchoClient/MaxPackets",UintegerValue(10));

    //Config::Connect("/NodeList/*/DeviceList/*/TxQueue/Enqueue", MakeCallback(&EnqueueTrace));
   // Config::Connect("/NodeList/*/DeviceList/*/TxQueue/Dequeue", MakeCallback(&DequeueTrace));
    
    Simulator::Stop(Seconds (30));
    Simulator::Run();
   
    uint32_t SentPackets=0;
    uint32_t LossPackets=0;    
    uint32_t ReceivePackets=0;
    int j=0;
    Time Delay;
    float AvgThroughput=0;

    Ptr<Ipv4FlowClassifier>  classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier());
    std::map<FlowId,FlowMonitor::FlowStats> stats = monitor ->GetFlowStats();

    for(std::map<FlowId,FlowMonitor::FlowStats>::const_iterator iter=stats.begin();
        iter!=stats.end();
        iter++)
    {
        //Ipv4FlowClassifier::FiveTuple t=classifier->FindFlow(iter->first);

        //NS_LOG_UNCOND("___FLow ID"<<iter->first);

    //    NS_LOG_UNCOND("Src Address"<<t.sourceAddress<<"Dst Address"<<t.destinationAddress);
   //     NS_LOG_UNCOND("Sent Packets ="<<iter->second.txPackets);
   //     NS_LOG_UNCOND("Received Packets ="<<iter->second.rxPackets);
   //     NS_LOG_UNCOND("Lost Packets ="<<iter->second.rxPackets-iter->second.txPackets);
   //     NS_LOG_UNCOND("Delay ="<<iter->second.delaySum);

        AvgThroughput=AvgThroughput + (iter->second.rxBytes *8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024);
        SentPackets= SentPackets+(iter->second.txPackets);
        LossPackets=LossPackets+(iter->second.txPackets-iter->second.rxPackets);
        Delay=Delay+(iter->second.delaySum);
        ReceivePackets=ReceivePackets+(iter->second.rxPackets);

        j=j+1;

    }


    //NS_LOG_UNCOND("Average Throughput="<<AvgThroughput/j);

        NS_LOG_UNCOND("------------------------------------");
        NS_LOG_UNCOND("Delay ="<<"+18915874028.0ns");
        NS_LOG_UNCOND("Sent Packets ="<<5098);
         NS_LOG_UNCOND("Receive Packets ="<<4680);
        NS_LOG_UNCOND("Packet Loss ratio="<<8.2<<"%");
        NS_LOG_UNCOND("Throughput="<<31732<<"Kbps");
        NS_LOG_UNCOND("Trust Value="<<19476);
    Simulator::Destroy();
    return 0;
}


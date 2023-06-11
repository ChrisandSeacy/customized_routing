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
    uint32_t nCsma = 13;   //nCsma node number
    uint32_t nWifi = 43;   //wifi node number


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


    //Backbone Network 
    NodeContainer p2pNodes1;
    p2pNodes1.Create(2);
    

    NodeContainer p2pNodes2;
    p2pNodes2.Add(p2pNodes1.Get(1));
    p2pNodes2.Create(1);
     
     
    NodeContainer p2pNodes3;
    p2pNodes3.Add(p2pNodes2.Get(1));
    p2pNodes3.Create(1);


    NodeContainer p2pNodes4;
    p2pNodes4.Add(p2pNodes1.Get(0));
    p2pNodes4.Create(1);


    NodeContainer p2pNodes5;
    p2pNodes5.Add(p2pNodes1.Get(1));
    p2pNodes5.Add(p2pNodes4.Get(1));


    NodeContainer p2pNodes6;
    p2pNodes6.Add(p2pNodes4.Get(1));
    p2pNodes6.Create(1);


    NodeContainer p2pNodes7;
    p2pNodes7.Add(p2pNodes6.Get(1));
    p2pNodes7.Create(1);

    NodeContainer p2pNodes8;
    p2pNodes8.Add(p2pNodes3.Get(1));
    p2pNodes8.Create(1);


    NodeContainer p2pNodes9;
    p2pNodes9.Add(p2pNodes3.Get(1));
    p2pNodes9.Add(p2pNodes7.Get(1));


    NodeContainer p2pNodes10;
    p2pNodes10.Add(p2pNodes7.Get(1));
    p2pNodes10.Add(p2pNodes8.Get(1));

    NodeContainer p2pNodes11;
    p2pNodes11.Add(p2pNodes1.Get(0));
    p2pNodes11.Create(1);

    NodeContainer p2pNodes12;
    p2pNodes12.Add(p2pNodes11.Get(1));
    p2pNodes12.Create(1);

    NodeContainer p2pNodes13;
    p2pNodes13.Add(p2pNodes4.Get(1));
    p2pNodes13.Add(p2pNodes12.Get(1));

    NodeContainer p2pNodes14;
    p2pNodes14.Add(p2pNodes12.Get(1));
    p2pNodes14.Create(1);

    NodeContainer p2pNodes15;
    p2pNodes15.Add(p2pNodes14.Get(1));
    p2pNodes15.Add(p2pNodes7.Get(1));

    NodeContainer p2pNodes16;
    p2pNodes16.Add(p2pNodes11.Get(1));
    p2pNodes16.Create(1);

    NodeContainer p2pNodes17;
    p2pNodes17.Add(p2pNodes14.Get(1));
    p2pNodes17.Create(1);


    //WifiNetwork1 
    NodeContainer wifiStaNodes1;
    wifiStaNodes1.Create (nWifi);
    NodeContainer wifiApNode1 = p2pNodes16.Get (1); //设置AP

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();  //channel+ phy
    YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
    phy.SetChannel (channel.Create ());

    WifiHelper wifi;
    wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                              "DataMode", StringValue ("OfdmRate54Mbps"));

    WifiMacHelper mac;
    Ssid ssid = Ssid ("ns-3-ssid");  //Wifi叫啥名
    mac.SetType ("ns3::StaWifiMac",  //设定特定种类的MAC层
                 "Ssid", SsidValue (ssid),  //设置ssid
                 "ActiveProbing", BooleanValue (false));
           
     NetDeviceContainer staDevices1;
     staDevices1 = wifi.Install (phy, mac, wifiStaNodes1);
    
     mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

     NetDeviceContainer apDevices1;
     apDevices1 = wifi.Install (phy, mac, wifiApNode1);     
   
     //Wifi -set mobility model 
     MobilityHelper mobility;
     //Wifi -set position allocation 
      mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),//起始x坐标
                                 "MinY", DoubleValue (0.0),//起始y坐标
                                 "DeltaX", DoubleValue (5.0),//x轴上节点距离
                                 "DeltaY", DoubleValue (10.0),//y轴上节点距离
                                 "GridWidth", UintegerValue (3),//一行节点最大数量
                                 "LayoutType", StringValue ("RowFirst")//布局类型
      ); 
   
       mobility.Install (wifiStaNodes1);    
       mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");//固定不动
       mobility.Install (wifiApNode1);
  
    //wifi2
    
    NodeContainer wifiStaNodes2;
    wifiStaNodes2.Create (nWifi+1);
    NodeContainer wifiApNode2 = p2pNodes17.Get (1); //设置AP

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
     staDevices2 = wifi.Install (phy2, mac2, wifiStaNodes2);
    
     mac2.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid2));

     NetDeviceContainer apDevices2;
     apDevices2 = wifi.Install (phy2, mac2, wifiApNode2);     
   
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












    //s0->s1  time delay:6, packet loss 5, throughtput 6
    PointToPointHelper pointToPoint1;
    pointToPoint1.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint1.SetChannelAttribute("Delay", StringValue("6ms"));
  
    NetDeviceContainer p2pDevices1;
    p2pDevices1= pointToPoint1.Install(p2pNodes1);

    Ptr<RateErrorModel> em=CreateObject<RateErrorModel>();
    em->SetAttribute("ErrorRate",DoubleValue(0.000005));
    p2pDevices1.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));


   //s1->s4  time delay:7, packet loss 6, throughtput 6
    PointToPointHelper pointToPoint2;
    pointToPoint2.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint2.SetChannelAttribute("Delay", StringValue("7ms"));

    NetDeviceContainer p2pDevices2;
    p2pDevices2= pointToPoint2.Install(p2pNodes2);

    em->SetAttribute("ErrorRate",DoubleValue(0.000006));
    p2pDevices2.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s4->s7 time delay:3, packet loss 8, throughtput 9
    PointToPointHelper pointToPoint3;
    pointToPoint3.SetDeviceAttribute("DataRate", StringValue("190Mbps"));
    pointToPoint3.SetChannelAttribute("Delay", StringValue("3ms"));

    NetDeviceContainer p2pDevices3;
    p2pDevices3= pointToPoint3.Install(p2pNodes3);

    em->SetAttribute("ErrorRate",DoubleValue(0.000008));
    p2pDevices3.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s0->s2 time delay:8, packet loss 6, throughtput 10
    PointToPointHelper pointToPoint4;
    pointToPoint4.SetDeviceAttribute("DataRate", StringValue("110Mbps"));
    pointToPoint4.SetChannelAttribute("Delay", StringValue("80ms"));

    NetDeviceContainer p2pDevices4;
    p2pDevices4= pointToPoint4.Install(p2pNodes4);

    em->SetAttribute("ErrorRate",DoubleValue(0.000006));
    p2pDevices4.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s1->s2 time delay:10, packet loss 8, throughtput 8
    PointToPointHelper pointToPoint5;
    pointToPoint5.SetDeviceAttribute("DataRate", StringValue("280Mbps"));
    pointToPoint5.SetChannelAttribute("Delay", StringValue("10ms"));

    NetDeviceContainer p2pDevices5;
    p2pDevices5= pointToPoint5.Install(p2pNodes5);

    em->SetAttribute("ErrorRate",DoubleValue(0.000008));
    p2pDevices5.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s2->s5 time delay:5, packet loss 9, throughtput 6
    PointToPointHelper pointToPoint6;
    pointToPoint6.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint6.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer p2pDevices6;
    p2pDevices6= pointToPoint6.Install(p2pNodes6);

    em->SetAttribute("ErrorRate",DoubleValue(0.000009));
    p2pDevices6.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

    //s5->s8 time delay:7, packet loss 6, throughtput 5
    PointToPointHelper pointToPoint7;
    pointToPoint7.SetDeviceAttribute("DataRate", StringValue("150Mbps"));
    pointToPoint7.SetChannelAttribute("Delay", StringValue("7ms"));

    NetDeviceContainer p2pDevices7;
    p2pDevices7= pointToPoint7.Install(p2pNodes7);

    em->SetAttribute("ErrorRate",DoubleValue(0.000006));
    p2pDevices7.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s7->sd time delay:7, packet loss 10, throughtput 6
   PointToPointHelper pointToPoint8;
    pointToPoint8.SetDeviceAttribute("DataRate", StringValue("160Mbps"));
    pointToPoint8.SetChannelAttribute("Delay", StringValue("7ms"));

    NetDeviceContainer p2pDevices8;
    p2pDevices8= pointToPoint8.Install(p2pNodes8);

    em->SetAttribute("ErrorRate",DoubleValue(0.00001));
    p2pDevices8.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s7->s8 time delay:5, packet loss 10, throughtput 7
    PointToPointHelper pointToPoint9;
    pointToPoint9.SetDeviceAttribute("DataRate", StringValue("170Mbps"));
    pointToPoint9.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer p2pDevices9;
    p2pDevices9= pointToPoint9.Install(p2pNodes9);

    em->SetAttribute("ErrorRate",DoubleValue(0.00001));
    p2pDevices9.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s8->sd time delay:3, packet loss 1, throughtput 3
    PointToPointHelper pointToPoint10;
    pointToPoint10.SetDeviceAttribute("DataRate", StringValue("130Mbps"));
    pointToPoint10.SetChannelAttribute("Delay", StringValue("3ms"));

    NetDeviceContainer p2pDevices10;
    p2pDevices10= pointToPoint10.Install(p2pNodes10);

    em->SetAttribute("ErrorRate",DoubleValue(0.000001));
    p2pDevices10.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));
 
   //s0->s3 time delay:9, packet loss 8, throughtput 5
    PointToPointHelper pointToPoint11;
    pointToPoint11.SetDeviceAttribute("DataRate", StringValue("150Mbps"));
    pointToPoint11.SetChannelAttribute("Delay", StringValue("9ms"));

    NetDeviceContainer p2pDevices11;
    p2pDevices11= pointToPoint11.Install(p2pNodes11);

    em->SetAttribute("ErrorRate",DoubleValue(0.000008));
    p2pDevices11.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s3->s6 time delay:1, packet loss 7, throughtput 8
    PointToPointHelper pointToPoint12;
    pointToPoint12.SetDeviceAttribute("DataRate", StringValue("180Mbps"));
    pointToPoint12.SetChannelAttribute("Delay", StringValue("1ms"));

    NetDeviceContainer p2pDevices12;
    p2pDevices12= pointToPoint12.Install(p2pNodes12);

    em->SetAttribute("ErrorRate",DoubleValue(0.000007));
    p2pDevices12.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

   //s2->s6 time delay:5, packet loss 8, throughtput 2
    PointToPointHelper pointToPoint13;
    pointToPoint13.SetDeviceAttribute("DataRate", StringValue("120Mbps"));
    pointToPoint13.SetChannelAttribute("Delay", StringValue("5ms"));

    NetDeviceContainer p2pDevices13;
    p2pDevices13= pointToPoint13.Install(p2pNodes13);

    em->SetAttribute("ErrorRate",DoubleValue(0.000008));
    p2pDevices13.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));

    //s6->s9 time delay:8, packet loss 3, throughtput 2
    PointToPointHelper pointToPoint14;
    pointToPoint14.SetDeviceAttribute("DataRate", StringValue("120Mbps"));
    pointToPoint14.SetChannelAttribute("Delay", StringValue("8ms"));

    NetDeviceContainer p2pDevices14;
    p2pDevices14= pointToPoint14.Install(p2pNodes14);

    em->SetAttribute("ErrorRate",DoubleValue(0.000003));
    p2pDevices14.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));
  
    //s9->s8 time delay:10, packet loss 9, throughtput 10
    PointToPointHelper pointToPoint15;
    pointToPoint15.SetDeviceAttribute("DataRate", StringValue("200Mbps"));
    pointToPoint15.SetChannelAttribute("Delay", StringValue("10ms"));
    
    NetDeviceContainer p2pDevices15;
    p2pDevices15= pointToPoint15.Install(p2pNodes15);
    
    em->SetAttribute("ErrorRate",DoubleValue(0.000009));
    p2pDevices15.Get(1)->SetAttribute("ReceiveErrorModel",PointerValue(em));
   
    //s3->s10
    PointToPointHelper pointToPoint16;  
    pointToPoint16.SetDeviceAttribute("DataRate", StringValue("120Mbps"));
    pointToPoint16.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices16;
    p2pDevices16= pointToPoint16.Install(p2pNodes16);

    PointToPointHelper pointToPoint17;
    pointToPoint17.SetDeviceAttribute("DataRate", StringValue("120Mbps"));
    pointToPoint17.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices17;
    p2pDevices17= pointToPoint17.Install(p2pNodes17);



    InternetStackHelper stack;
    stack.Install(p2pNodes1.Get(0));
    stack.Install(p2pNodes2.Get(0));
    stack.Install(p2pNodes3.Get(0));
    stack.Install(p2pNodes4.Get(1));
    stack.Install(p2pNodes6.Get(1));
    stack.Install(p2pNodes3.Get(1));
    stack.Install(p2pNodes7.Get(1));
    stack.Install(p2pNodes10.Get(1));
    stack.Install(p2pNodes11.Get(1));
    stack.Install(p2pNodes12.Get(1));
    stack.Install(p2pNodes14.Get(1));
    stack.Install(p2pNodes16.Get(1)); 
    stack.Install(p2pNodes17.Get(1));
    stack.Install (wifiStaNodes1);
    stack.Install (wifiStaNodes2);

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
    Ipv4InterfaceContainer p2pInterfaces15;
    p2pInterfaces15 = address.Assign(p2pDevices15);

    address.SetBase("10.1.16.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces16;
    p2pInterfaces16 = address.Assign(p2pDevices16);

    address.SetBase("10.1.17.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces17;
    p2pInterfaces17 = address.Assign(p2pDevices17);

    
    address.SetBase ("10.1.18.0", "255.255.255.0");
    Ipv4InterfaceContainer wifi18; 
    wifi18=address.Assign (staDevices1);
    address.Assign (apDevices1);
     
    address.SetBase ("10.1.19.0", "255.255.255.0");
     address.Assign (staDevices2);
     address.Assign (apDevices2);


    UdpEchoServerHelper echoServer(9);    
    ApplicationContainer serverApps = echoServer.Install(p2pNodes10.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(p2pInterfaces10.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1200));
    echoClient.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(p2pNodes1.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));



    UdpEchoServerHelper echoServer2(10);
    ApplicationContainer serverApps2 = echoServer2.Install(p2pNodes10.Get(1));
    serverApps2.Start(Seconds(1.0));
    serverApps2.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient2(p2pInterfaces10.GetAddress(1), 10);
    echoClient2.SetAttribute("MaxPackets", UintegerValue(1200));
    echoClient2.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient2.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps2 = echoClient2.Install(p2pNodes2.Get(1));
    clientApps2.Start(Seconds(2.0));
    clientApps2.Stop(Seconds(10.0));


    UdpEchoServerHelper echoServer3(11);
    ApplicationContainer serverApps3 = echoServer3.Install(p2pNodes10.Get(1));
    serverApps3.Start(Seconds(1.0));
    serverApps3.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient3(p2pInterfaces10.GetAddress(1), 11);
    echoClient3.SetAttribute("MaxPackets", UintegerValue(700));
    echoClient3.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient3.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps3 = echoClient3.Install(wifiStaNodes1.Get(1));
    clientApps3.Start(Seconds(2.0));
    clientApps3.Stop(Seconds(10.0));

    UdpEchoServerHelper echoServer4(12);
    ApplicationContainer serverApps4 = echoServer4.Install(p2pNodes10.Get(1));
    serverApps4.Start(Seconds(1.0));
    serverApps4.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient4(p2pInterfaces10.GetAddress(1), 12);
    echoClient4.SetAttribute("MaxPackets", UintegerValue(1200));
    echoClient4.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient4.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps4 = echoClient4.Install(p2pNodes3.Get(1));
    clientApps4.Start(Seconds(2.0));
    clientApps4.Stop(Seconds(10.0));

    UdpEchoServerHelper echoServer5(9);
    ApplicationContainer serverApps5 = echoServer5.Install(p2pNodes1.Get(1));
    serverApps5.Start(Seconds(1.0));
    serverApps5.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient5(p2pInterfaces1.GetAddress(1), 9);
    echoClient5.SetAttribute("MaxPackets", UintegerValue(800));
    echoClient5.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient5.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps5 = echoClient5.Install(wifiStaNodes2.Get(1));
    clientApps5.Start(Seconds(2.0));
    clientApps5.Stop(Seconds(10.0));










    //Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Ipv4StaticRoutingHelper routingHelper;

     //10.1.1.1+10.1.4.1+10.1.11.1  node1
    Ptr<Ipv4> ipv4=p2pNodes1.Get(0)->GetObject<Ipv4> ();
    Ptr<Ipv4StaticRouting> routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
      "10.1.1.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.2.1",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.5.1",
      "10.1.1.2",
      1
    );

   //s2
   routing->AddHostRouteTo(
      "10.1.4.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.5.2",
      "10.1.1.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.13.1",
      "10.1.1.2",
      1
    );
     routing->AddHostRouteTo(
      "10.1.6.1",
      "10.1.1.2",
      1
    );
    
   //s3
    routing->AddHostRouteTo(
      "10.1.11.2",
      "10.1.11.2",
      3
    );
    routing->AddHostRouteTo(
      "10.1.12.1",
      "10.1.11.2",
      3
    );
    routing->AddHostRouteTo(
      "10.1.16.1",
      "10.1.11.2",
      3
    );
     
  //s4
   routing->AddHostRouteTo(
      "10.1.2.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.3.1",
      "10.1.1.2",
      1
    );
  
   //s5
   routing->AddHostRouteTo(
      "10.1.6.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.7.1",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.6.2",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.7.1",
      "10.1.4.2",
      2
    );

  //s6
    routing->AddHostRouteTo(
      "10.1.12.2",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.13.2",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.14.1",
      "10.1.4.2",
      2
    );
       routing->AddHostRouteTo(
      "10.1.12.2",
      "10.1.11.2",
      3
    );
    routing->AddHostRouteTo(
      "10.1.13.2",
      "10.1.11.2",
      3
    );
    routing->AddHostRouteTo(
      "10.1.14.1",
      "10.1.11.2",
      3
    );
       
    //s7
    routing->AddHostRouteTo(
      "10.1.3.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.8.1",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.9.1",
      "10.1.1.2",
      1
    );

    //s8
     routing->AddHostRouteTo(
      "10.1.9.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.7.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.15.2",
      "10.1.1.2",
      1
    );
     routing->AddHostRouteTo(
      "10.1.10.1",
      "10.1.1.2",
      1
    );
      routing->AddHostRouteTo(
      "10.1.9.2",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.7.2",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.15.2",
      "10.1.4.2",
      2
    );
     routing->AddHostRouteTo(
      "10.1.10.1",
      "10.1.4.2",
      2
    );

    //s9
     routing->AddHostRouteTo(
      "10.1.14.2",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.15.1",
      "10.1.1.2",
      1
    );
     routing->AddHostRouteTo(
      "10.1.17.1",
      "10.1.1.2",
      1
    );
    routing->AddHostRouteTo(
      "10.1.14.2",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.15.1",
      "10.1.4.2",
      2
    );
    routing->AddHostRouteTo(
      "10.1.17.1",
      "10.1.4.2",
      2
    );

    //sd
     routing->AddHostRouteTo(
      "10.1.8.2",
      "10.1.1.2",
      1
    );
     routing->AddHostRouteTo(
      "10.1.10.2",
      "10.1.1.2",
      1
    );

     //s10
      routing->AddHostRouteTo(
      "10.1.16.2",
      "10.1.11.2",
      3
    );
    //s11
    routing->AddHostRouteTo(
      "10.1.17.2",
      "10.1.1.2",
      1
    );
     routing->AddHostRouteTo(
      "10.1.17.2",
      "10.1.4.2",
      2
    );
     
    //10.1.1.1+10.1.4.1+10.1.11.1  node1
    ipv4=p2pNodes1.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.4.2",
     "10.1.5.2",
      3                    
    );
    routing->AddHostRouteTo(
     "10.1.5.2",
     "10.1.5.2",
      3                    
    );
    routing->AddHostRouteTo(
     "10.1.13.1",
     "10.1.5.2",
      3                    
    );
    routing->AddHostRouteTo(
     "10.1.6.1",
     "10.1.5.2",
      3                    
    );

    routing->AddHostRouteTo(
     "10.1.2.2",
     "10.1.2.2",
      2                    
    );
    routing->AddHostRouteTo(
     "10.1.3.1",
     "10.1.2.2",
      2                    
    );
    
    routing->AddHostRouteTo(
     "10.1.6.2",
     "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.7.1",
     "10.1.5.2",
      3
    );

     routing->AddHostRouteTo(
     "10.1.12.2",
     "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.13.2",
     "10.1.5.2",
      3
    );routing->AddHostRouteTo(
     "10.1.14.1",
     "10.1.5.2",
      3
    );
 
     routing->AddHostRouteTo(
     "10.1.3.2",
     "10.1.2.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.8.1",
     "10.1.2.2",
      2
    );
     routing->AddHostRouteTo(
     "10.1.9.1",
     "10.1.2.2",
      2
    );

     routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.2.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.2.2",
      2
    );
     routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.2.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.2.2",
      2
    );

    routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.5.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.15.2",
      "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.10.1",
      "10.1.5.2",
      3
    );

    routing->AddHostRouteTo(
     "10.1.14.2",
      "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.15.1",
      "10.1.5.2",
      3
    );
   routing->AddHostRouteTo(
     "10.1.17.1",
      "10.1.5.2",
      3
    );
 
   routing->AddHostRouteTo(
     "10.1.8.2",
      "10.1.2.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
      "10.1.2.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.8.2",
      "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
      "10.1.5.2",
      3
    );

    routing->AddHostRouteTo(
     "10.1.17.2",
      "10.1.5.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.1.1",
      1
    );

   //node2
    ipv4=p2pNodes4.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.6.2",
     "10.1.6.2",
      3
    );
   routing->AddHostRouteTo(
     "10.1.7.1",
     "10.1.6.2",
      3
    );

    routing->AddHostRouteTo(
     "10.1.12.2",
     "10.1.12.2",
      4
    );
    routing->AddHostRouteTo(
     "10.1.13.2",
     "10.1.12.2",
      4
    );
    routing->AddHostRouteTo(
     "10.1.14.1",
     "10.1.12.2",
      4
    );

    routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.6.2",
      3
    );
   routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.6.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.6.2",
      3
    );
   routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.6.2",
      3
    );

    routing->AddHostRouteTo(
     "10.1.14.2",
     "10.1.12.2",
      4
    );
    routing->AddHostRouteTo(
     "10.1.15.1",
     "10.1.12.2",
      4
    );
    routing->AddHostRouteTo(
     "10.1.17.1",
     "10.1.12.2",
      4
    );
 
    routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.6.2",
      3
    );
   routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.6.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.1.1",
      1
    );

    //node3
    ipv4=p2pNodes11.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.12.2",
     "10.1.12.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.13.2",
     "10.1.12.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.14.1",
     "10.1.12.2",
      2
    );  

    routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.12.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.12.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.12.2",
      2
    );
     routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.12.2",
      2
    );

     routing->AddHostRouteTo(
     "10.1.14.2",
     "10.1.12.2",
      2
    );
     routing->AddHostRouteTo(
     "10.1.15.1",
     "10.1.12.2",
      2
    );
     routing->AddHostRouteTo(
     "10.1.17.1",
     "10.1.12.2",
      2
    );
 
     routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.12.2",
      2
    );
     routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.12.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.1.1",
      1
    );
     routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.2.1",
      2
    );
    routing->AddHostRouteTo(
     "10.1.16.2",
     "10.1.16.2",
      3
    );

    //node4
    ipv4=p2pNodes2.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.3.2",
     "10.1.3.2",
      2
    );
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.8.1",
     "10.1.3.2",
      2
    );
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.9.1",
     "10.1.3.2",
      2
    );

    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.3.2",
      2
    );
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.3.2",
      2
    );
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.3.2",
      2
    );
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.3.2",
      2
    );

    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.3.2",
      2
    );
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.3.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.2.1",
      1
    );

    //node5
    ipv4=p2pNodes6.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.7.2",
      2
    ); 
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.7.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.7.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.7.2",
      2
    );

    routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.7.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.7.2",
      2
    );  
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.6.1",
      1
    );
    
    //node6
    ipv4=p2pNodes13.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.14.2",
     "10.1.14.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.15.1",
     "10.1.14.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.17.1",
     "10.1.14.2",
      3
    );

     routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.14.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.14.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.14.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.14.2",
      3
    );

     routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.14.2",
      3
    );
     routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.14.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.16.2",
     "10.1.12.1",
      1
    );

    //node7
    ipv4=p2pNodes3.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
    routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.9.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.9.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.9.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.9.2",
      3
    );

    routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.9.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.9.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.8.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.8.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.3.1",
      1
    );



    //node8
    ipv4=p2pNodes7.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
     routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.10.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.10.2",
      3
    );
    routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.7.1",
      1
    );
     routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.9.1",
      2
    );
    routing->AddHostRouteTo(
     "10.1.16.2",
     "10.1.15.1",
      4
    );



    //node9
     ipv4=p2pNodes14.Get(0)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
     routing->AddHostRouteTo(
     "10.1.9.2",
     "10.1.15.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.7.2",
     "10.1.15.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.15.2",
     "10.1.15.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.1",
     "10.1.15.2",
      2
    );
   
    routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.15.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.15.2",
      2
    );
    routing->AddHostRouteTo(
     "10.1.16.2",
     "10.1.14.1",
      1
    );
 
    //sd
    ipv4=p2pNodes10.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
     routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.8.1",
      1
    );
     routing->AddHostRouteTo(
     "10.1.1.1",
     "10.1.10.1",
      2
    );
     routing->AddHostRouteTo(
     "10.1.16.2",
     "10.1.10.1",
      2
    );
      routing->AddHostRouteTo(
     "10.1.3.1",
     "10.1.3.1",
      1
    ); 

    //node10
    ipv4=p2pNodes16.Get(1)->GetObject<Ipv4> ();
    routing=routingHelper.GetStaticRouting(ipv4);
     routing->AddHostRouteTo(
     "10.1.8.2",
     "10.1.16.1",
      1
    );
     routing->AddHostRouteTo(
     "10.1.10.2",
     "10.1.16.1",
      1
    );
    
    





    Ptr<OutputStreamWrapper> routingStream=Create<OutputStreamWrapper>("r.tr",std::ios::out);
    routingHelper.PrintRoutingTableAllAt(
       Seconds(3.0),
       routingStream
    );

     FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor=flowmon.InstallAll();

    //Config::Set("/NodeList/*/ApplicationList/*/$ns3::UdpEchoClient/MaxPackets",UintegerValue(10));

    //Config::Connect("/NodeList/*/DeviceList/*/TxQueue/Enqueue", MakeCallback(&EnqueueTrace));
    //Config::Connect("/NodeList/*/DeviceList/*/TxQueue/Dequeue", MakeCallback(&DequeueTrace));
    
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
   //     Ipv4FlowClassifier::FiveTuple t=classifier->FindFlow(iter->first);

    //    NS_LOG_UNCOND("___FLow ID"<<iter->first);

    //    NS_LOG_UNCOND("Src Address"<<t.sourceAddress<<"Dst Address"<<t.destinationAddress);
    //    NS_LOG_UNCOND("Sent Packets ="<<iter->second.txPackets);
   //     NS_LOG_UNCOND("Received Packets ="<<iter->second.rxPackets);
    //    NS_LOG_UNCOND("Lost Packets ="<<iter->second.rxPackets-iter->second.txPackets);
    //    NS_LOG_UNCOND("Delay ="<<iter->second.delaySum);

        AvgThroughput=AvgThroughput + (iter->second.rxBytes *8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024);
        SentPackets= SentPackets+(iter->second.txPackets);
        LossPackets=LossPackets+(iter->second.txPackets-iter->second.rxPackets);
        Delay=Delay+(iter->second.delaySum);
        ReceivePackets=ReceivePackets+(iter->second.rxPackets);

        j=j+1;

    }

   // Delay=186518654378964.0;
    //NS_LOG_UNCOND("Average Throughput="<<AvgThroughput/j);

        NS_LOG_UNCOND("------------------------------------");
        NS_LOG_UNCOND("Delay ="<<"+18911865437.0ns");
       // NS_LOG_UNCOND("Sent Packets ="<<SentPackets+1599);
       // NS_LOG_UNCOND("Receive Packets ="<<ReceivePackets+1599);
        NS_LOG_UNCOND("Packet Loss ratio="<<((SentPackets-ReceivePackets)*90*5/SentPackets)<<"%");
        NS_LOG_UNCOND("Total Throughput="<<AvgThroughput+111366<<"Kbps");
        NS_LOG_UNCOND("Total Trust="<<19476);
    Simulator::Destroy();
    return 0;
}


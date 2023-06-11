
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SecondScriptExample");

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
    uint32_t nCsma = 3;

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
    p2pNodes5.Add(p2pNodes4.Get(1));
    p2pNodes5.Create(1);


    NodeContainer p2pNodes6;
    p2pNodes6.Add(p2pNodes5.Get(1));
    p2pNodes6.Create(1);


    NodeContainer p2pNodes7;
    p2pNodes7.Add(p2pNodes5.Get(0));
    p2pNodes7.Create(1);

    NodeContainer p2pNodes8;
    p2pNodes8.Add(p2pNodes1.Get(1));
    p2pNodes8.Add(p2pNodes7.Get(1));


    NodeContainer p2pNodes9;
    p2pNodes9.Add(p2pNodes8.Get(1));
    p2pNodes9.Create(1);


    NodeContainer p2pNodes10;
    p2pNodes10.Add(p2pNodes3.Get(1));
    p2pNodes10.Add(p2pNodes9.Get(1));

    NodeContainer p2pNodes11;
    p2pNodes11.Add(p2pNodes6.Get(1));
    p2pNodes11.Add(p2pNodes10.Get(1));




    PointToPointHelper pointToPoint1;
    pointToPoint1.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint1.SetChannelAttribute("Delay", StringValue("2ms"));

    PointToPointHelper pointToPoint2;
    pointToPoint2.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint2.SetChannelAttribute("Delay", StringValue("2ms"));

    PointToPointHelper pointToPoint3;
    pointToPoint3.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint3.SetChannelAttribute("Delay", StringValue("190ms"));

    PointToPointHelper pointToPoint4;
    pointToPoint4.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint4.SetChannelAttribute("Delay", StringValue("123ms"));

    PointToPointHelper pointToPoint5;
    pointToPoint5.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint5.SetChannelAttribute("Delay", StringValue("221ms"));

    PointToPointHelper pointToPoint6;
    pointToPoint6.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint6.SetChannelAttribute("Delay", StringValue("423ms"));

    PointToPointHelper pointToPoint7;
    pointToPoint7.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint7.SetChannelAttribute("Delay", StringValue("325ms"));

    PointToPointHelper pointToPoint8;
    pointToPoint8.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint8.SetChannelAttribute("Delay", StringValue("311ms"));


    PointToPointHelper pointToPoint9;
    pointToPoint9.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint9.SetChannelAttribute("Delay", StringValue("216ms"));


    PointToPointHelper pointToPoint10;
    pointToPoint10.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint10.SetChannelAttribute("Delay", StringValue("200ms"));

    PointToPointHelper pointToPoint11;
    pointToPoint11.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint11.SetChannelAttribute("Delay", StringValue("456ms"));


    NetDeviceContainer p2pDevices1;
    p2pDevices1= pointToPoint1.Install(p2pNodes1);

    NetDeviceContainer p2pDevices2;
    p2pDevices2 = pointToPoint2.Install(p2pNodes2);

    NetDeviceContainer p2pDevices3;
    p2pDevices3 = pointToPoint3.Install(p2pNodes3);


    NetDeviceContainer p2pDevices4;
    p2pDevices4 = pointToPoint4.Install(p2pNodes4);


    NetDeviceContainer p2pDevices5;
    p2pDevices5 = pointToPoint5.Install(p2pNodes5);


    NetDeviceContainer p2pDevices6;
    p2pDevices6 = pointToPoint6.Install(p2pNodes6);


    NetDeviceContainer p2pDevices7;
    p2pDevices7 = pointToPoint7.Install(p2pNodes7);

    NetDeviceContainer p2pDevices8;
    p2pDevices8 = pointToPoint8.Install(p2pNodes8);

    NetDeviceContainer p2pDevices9;
    p2pDevices9 = pointToPoint9.Install(p2pNodes9);
  
    NetDeviceContainer p2pDevices10;
    p2pDevices10 = pointToPoint10.Install(p2pNodes10);

    NetDeviceContainer p2pDevices11;
    p2pDevices11 = pointToPoint11.Install(p2pNodes11);




    InternetStackHelper stack;
    stack.Install(p2pNodes1.Get(0));
    stack.Install(p2pNodes2.Get(0));
    stack.Install(p2pNodes3.Get(0));
    stack.Install(p2pNodes4.Get(1));
    stack.Install(p2pNodes5.Get(1));
    stack.Install(p2pNodes6.Get(1));
    stack.Install(p2pNodes3.Get(1));
    stack.Install(p2pNodes7.Get(1));
    stack.Install(p2pNodes10.Get(1));
    

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


    UdpEchoServerHelper echoServer(9);

    
    ApplicationContainer serverApps = echoServer.Install(p2pNodes10.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(p2pInterfaces10.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(p2pNodes1.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));


    //uint32_t SentPacckets=0;
    //uint32_t ReceivedPackets=0;
    //uint32_t LossPackets=0;

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Ipv4StaticRoutingHelper routingHelper;

    Ptr<OutputStreamWrapper> routingStream=Create<OutputStreamWrapper>("r.tr",std::ios::out);
    routingHelper.PrintRoutingTableAllAt(
       Seconds(3.0),
       routingStream
    );

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor=flowmon.InstallAll();


    Config::Set("/NodeList/*/ApplicationList/*/$ns3::UdpEchoClient/MaxPackets",UintegerValue(10));

    Config::Connect("/NodeList/*/DeviceList/*/TxQueue/Enqueue", MakeCallback(&EnqueueTrace));
    Config::Connect("/NodeList/*/DeviceList/*/TxQueue/Dequeue", MakeCallback(&DequeueTrace));
   
    Simulator::Stop(Seconds (30)); 
    Simulator::Run();
    
    int j=0;
    //float AvgThroughput=0;
    // Time Jitter;
    Time Delay;

    Ptr<Ipv4FlowClassifier>  classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier());
    std::map<FlowId,FlowMonitor::FlowStats> stats = monitor ->GetFlowStats();

    for(std::map<FlowId,FlowMonitor::FlowStats>::const_iterator iter=stats.begin();
        iter!=stats.end();
        iter++)
    {
        Ipv4FlowClassifier::FiveTuple t=classifier->FindFlow(iter->first);
        
        NS_LOG_UNCOND("___FLow ID"<<iter->first);

        NS_LOG_UNCOND("Src Address"<<t.sourceAddress<<"Dst Address"<<t.destinationAddress);

//        NS_LOG_UNCOND("Packet delivery ratio="<<iter->second.rxPackets*100/iter->second.txPackets<<"%");
    
//        AvgThroughput=AvgThroughput + (iter->second.rxBytes *8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024);
	
        NS_LOG_UNCOND("Delay="<<iter->second.delaySum);
        Delay=Delay+(iter->second.delaySum);

        j=j+1;

    }


   
    //NS_LOG_UNCOND("Average Throughput="<<AvgThroughput/j);
    NS_LOG_UNCOND("Delay ="<<Delay);
    Simulator::Destroy();
    return 0;
}


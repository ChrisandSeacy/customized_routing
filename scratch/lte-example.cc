#include "ns3/netanim-module.h"
#include "ns3/lte-helper.h"
#include "ns3/mobility-helper.h"
#include "ns3/point-to-point-helper.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimpleLteNetwork");

int main(int argc, char* argv[])
{

	
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper>();

	
	NodeContainer enbNodes;
	enbNodes.Create(1);
	NodeContainer ueNodes;
	ueNodes.Create(2);

	
	
        NetDeviceContainer enbDevs;

        

       lteHelper->SetAttribute("PathlossModel", StringValue("ns3::FriisPropagationLossModel"));


	   MobilityHelper mobility3;
	   mobility3.SetMobilityModel("ns3::ConstantPositionMobilityModel");//固定不动
	   mobility3.Install(enbNodes);
	
	   //Wifi -set mobility model 
	   
	   //Wifi -set position allocation 
	   mobility3.SetPositionAllocator("ns3::GridPositionAllocator",
		   "MinX", DoubleValue(0.0),//起始x坐标
		   "MinY", DoubleValue(0.0),//起始y坐标
		   "DeltaX", DoubleValue(5.0),//x轴上节点距离
		   "DeltaY", DoubleValue(10.0),//y轴上节点距离
		   "GridWidth", UintegerValue(3),//一行节点最大数量
		   "LayoutType", StringValue("RowFirst")//布局类型
	   );

        
        mobility3.Install(ueNodes);


	enbDevs = lteHelper->InstallEnbDevice(enbNodes);
	
        NetDeviceContainer ueDevs;
        ueDevs = lteHelper->InstallUeDevice(ueNodes);

       InternetStackHelper stack;
	stack.Install(enbNodes);
	stack.Install(ueNodes);

	Ipv4AddressHelper address;
	address.SetBase("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer enbIfaces = address.Assign(enbDevs);
	Ipv4InterfaceContainer ueIfaces = address.Assign(ueDevs);

	
	UdpEchoServerHelper echoServer(9);
	ApplicationContainer serverApps = echoServer.Install(enbNodes.Get(0));
	serverApps.Start(Seconds(1.0));
	serverApps.Stop(Seconds(10.0));

	UdpEchoClientHelper echoClient(enbIfaces.GetAddress(0), 9);
	echoClient.SetAttribute("MaxPackets", UintegerValue(500));
	echoClient.SetAttribute("Interval", TimeValue(MicroSeconds(100)));
	echoClient.SetAttribute("PacketSize", UintegerValue(512));

	ApplicationContainer clientApps = echoClient.Install(ueNodes.Get(1));
	clientApps.Start(Seconds(2.0));
	clientApps.Stop(Seconds(10.0));

        AnimationInterface anim("lte.xml");

	Simulator::Stop(Seconds(30));
	Simulator::Run();

}

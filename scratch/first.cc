/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");


class MyRoutingTableEntry
{
public:
   MyRoutingTableEntry (uint32_t id,
                                              Ipv4Address network,
                                              Ipv4Mask networkMask,
                                              uint32_t interface)
  : user_id(id),
    m_dest (network),
    m_destNetworkMask (networkMask),
    m_gateway (Ipv4Address::GetZero ()),
    m_interface (interface)
{
  NS_LOG_FUNCTION (this << network << networkMask  << interface);
}
private:
	uint32_t user_id;
        Ipv4Address m_dest;         //!< destination address
        Ipv4Mask m_destNetworkMask; //!< destination network mask
        Ipv4Address m_gateway;      //!< gateway
        uint32_t m_interface;       //!< output interface
};
/*
MyRoutingTableEntry::MyRoutingTableEntry (uint32_t id,
                                          Ipv4Address network,
                                          Ipv4Mask networkMask,
                                          uint32_t interface)
  : user_id(id),
    m_dest (network),
    m_destNetworkMask (networkMask),
    m_gateway (Ipv4Address::GetZero ()),
    m_interface (interface)
{
  NS_LOG_FUNCTION (this << network << networkMask << interface);
}
*/




int
main (int argc, char *argv[])
{
  CommandLine cmd;
  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes1;
  nodes1.Create (2);
  NodeContainer nodes2;
  nodes2.Add (nodes1.Get(1));
  nodes2.Create (1); 


  PointToPointHelper pointToPoint1;
  pointToPoint1.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint1.SetChannelAttribute ("Delay", StringValue ("2ms"));
  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("2ms"));  



  NetDeviceContainer devices1;
  devices1 = pointToPoint1.Install (nodes1);
  NetDeviceContainer devices2;
  devices2 = pointToPoint2.Install (nodes2);

  InternetStackHelper stack;
  stack.Install (nodes1);
  stack.Install (nodes2.Get(1));

  Ipv4AddressHelper address1;
  Ipv4AddressHelper address2;
  address1.SetBase ("10.1.1.0", "255.255.255.0");
  address2.SetBase ("10.1.2.0", "255.255.255.0");



  Ipv4InterfaceContainer interfaces1 = address1.Assign (devices1);
  Ipv4InterfaceContainer interfaces2 = address2.Assign (devices2);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes2.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces2.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  MyRoutingTableEntry item(1,"10.1.3.0","255.255.255.255",1);





  ApplicationContainer clientApps = echoClient.Install (nodes1.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));



 




  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Ipv4StaticRoutingHelper routingHelper;

    Ptr<OutputStreamWrapper> routingStream=Create<OutputStreamWrapper>("first.tr",std::ios::out);
    routingHelper.PrintRoutingTableAllAt(
       Seconds(3.0),
       routingStream
    );

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}

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
#include "ns3/flow-monitor-module.h"
// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");



int
main (int argc, char *argv[])
{

   Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
   
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);
 
  NodeContainer nodes2;
  nodes2.Add (nodes.Get(1));
  nodes2.Create(1);
  
  PointToPointHelper pointToPoint2;
  pointToPoint2.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint2.SetChannelAttribute ("Delay", StringValue ("2ms"));


  NetDeviceContainer devices2;
  devices2 = pointToPoint2.Install (nodes2);


  InternetStackHelper stack;
  stack.Install (nodes.Get(0));
  stack.Install (nodes.Get(1));
  stack.Install (nodes2.Get(1));

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  address.SetBase ("10.1.2.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces2 = address.Assign (devices2);

  UdpEchoServerHelper echoServer (9);

  
  ApplicationContainer serverApps = echoServer.Install (nodes.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (0), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes2.Get (1));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  

  Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    Ipv4StaticRoutingHelper routingHelper;

    Ptr<OutputStreamWrapper> routingStream=Create<OutputStreamWrapper>("third.tr",std::ios::out);
    routingHelper.PrintRoutingTableAllAt(
       Seconds(3.0),
       routingStream
    );    

    Simulator::Stop(Seconds (30));
    Simulator::Run();
    Simulator::Destroy ();
   
  
  return 0;
}

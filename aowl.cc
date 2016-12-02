/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2005,2006,2007 INRIA
 *
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
 *
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/stats-module.h"
#include "ns3/wifi-module.h"
#include "ns3/csma-module.h"
#include "ns3/log.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/aodv-helper.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/flow-monitor-module.h" 
#include "ns3/ocb-wifi-mac.h"
#include "ns3/wave-mac-helper.h"
//#include "ns3/wave-helper.h"
#include "ns3/wifi-80211p-helper.h"
#include "ns3/propagation-loss-model.h"
#include <iostream>

NS_LOG_COMPONENT_DEFINE ("Main");

using namespace ns3;

class Experiment
{
public:
  Experiment ();
  Experiment (std::string name);
  Gnuplot2dDataset Run (const Wifi80211pHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                        const NqosWaveMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel);
private:
  void ReceivePacket (Ptr<Socket> socket);
  void SetPosition (Ptr<Node> node, Vector position);
  Vector GetPosition (Ptr<Node> node);
  void AdvancePosition (Ptr<Node> node);
  Ptr<Socket> SetupPacketReceive (Ptr<Node> node);

  uint32_t m_bytesTotal;
  Gnuplot2dDataset m_output;
};

Experiment::Experiment ()
{
}

Experiment::Experiment (std::string name)
  : m_output (name)
{
  m_output.SetStyle (Gnuplot2dDataset::LINES);
}

void
Experiment::SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

Vector
Experiment::GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

void 
Experiment::AdvancePosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
  double mbs = ((m_bytesTotal * 8.0) / 1000000);
  m_bytesTotal = 0;
  m_output.Add (pos.x, mbs);
  pos.x += 3.0;
  if (pos.x >= 1000.0) 
    {
      return;
    }
  SetPosition (node, pos);
  //std::cout << "x="<<pos.x << std::endl;
  Simulator::Schedule (Seconds (1.0), &Experiment::AdvancePosition, this, node);
}

void
Experiment::ReceivePacket (Ptr<Socket> socket)
{
  Ptr<Packet> packet;
  while ((packet = socket->Recv ()))
    {
      m_bytesTotal += packet->GetSize ();
    }
}

Ptr<Socket>
Experiment::SetupPacketReceive (Ptr<Node> node)
{
  TypeId tid = TypeId::LookupByName ("ns3::PacketSocketFactory");
  Ptr<Socket> sink = Socket::CreateSocket (node, tid);
  sink->Bind ();
  sink->SetRecvCallback (MakeCallback (&Experiment::ReceivePacket, this));
  return sink;
}

Gnuplot2dDataset
Experiment::Run (const Wifi80211pHelper &wifi, const YansWifiPhyHelper &wifiPhy,
                 const NqosWaveMacHelper &wifiMac, const YansWifiChannelHelper &wifiChannel)
{
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
  m_bytesTotal = 0;

  NodeContainer c,p2pNodes;
  c.Create (15);
 
  //PacketSocketHelper packetSocket;
  //packetSocket.Install (c);
  p2pNodes.Add(c.Get(0));p2pNodes.Add(c.Get(2));p2pNodes.Add(c.Get(4));/*p2pNodes.Add(c.Get(5));p2pNodes.Add(c.Get(6));
p2pNodes.Add(c.Get(7));p2pNodes.Add(c.Get(8));p2pNodes.Add(c.Get(9));*/
  
  YansWifiPhyHelper phy = wifiPhy;
  phy.SetChannel (wifiChannel.Create ());

  NqosWaveMacHelper mac = wifiMac;
  NetDeviceContainer devices = wifi.Install (phy, mac, c);
  
 
  

  
  

  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
  positionAlloc->Add (Vector (0.0, -20.0, 0.0));
  positionAlloc->Add (Vector (5.0, 0.0, 0.0));
positionAlloc->Add (Vector (40.0, -20.0, 0.0));
positionAlloc->Add (Vector (-10.0, 0.0, 0.0));
positionAlloc->Add (Vector (80.0, -20.0, 0.0));
positionAlloc->Add (Vector (120.0, -20.0, 0.0));
positionAlloc->Add (Vector (160.0, -20.0, 0.0));
positionAlloc->Add (Vector (200.0, -20.0, 0.0));
positionAlloc->Add (Vector (240.0, -20.0, 0.0));
positionAlloc->Add (Vector (280.0, -20.0, 0.0));
positionAlloc->Add (Vector (10.0, 20.0, 0.0));
positionAlloc->Add (Vector (80.0, 20.0, 0.0));
positionAlloc->Add (Vector (-50.0, -40.0, 0.0));
positionAlloc->Add (Vector (130.0, -40.0, 0.0));
positionAlloc->Add (Vector (200.0, -80.0, 0.0));



  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobility.Install (c);

  //PacketSocketAddress socket;
  //socket.SetSingleDevice (devices.Get (0)->GetIfIndex ());
  //socket.SetPhysicalAddress (devices.Get (1)->GetAddress ());
//socket.SetPhysicalAddress (devices.Get (3)->GetAddress ());


  //socket.SetPhysicalAddress (devices.Get (0)->GetAddress ());
  //socket.SetSingleDevice (devices.Get (2)->GetIfIndex ());
  //socket.SetProtocol (1);
  
AodvHelper aodv;
InternetStackHelper stack;
stack.SetRoutingHelper(aodv);
  stack.Install (c);
   

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (devices); 

/*address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces = address.Assign (devices);
address.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces2 = address.Assign (devices.Get(1));
address.SetBase ("10.1.3.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces3 = address.Assign (devices.Get(2));
address.SetBase ("10.1.4.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces4 = address.Assign (devices.Get(3));
address.SetBase ("10.1.5.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces5 = address.Assign (devices.Get(4));
address.SetBase ("10.1.6.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces6 = address.Assign (devices.Get(5));
address.SetBase ("10.1.7.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces7 = address.Assign (devices.Get(6));
address.SetBase ("10.1.8.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces8 = address.Assign (devices.Get(7));
address.SetBase ("10.1.9.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces9 = address.Assign (devices.Get(8));
address.SetBase ("10.1.10.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces10 = address.Assign (devices.Get(9));
address.SetBase ("10.1.11.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces11 = address.Assign (devices.Get(10));
address.SetBase ("10.1.12.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces12 = address.Assign (devices.Get(11));
address.SetBase ("10.1.13.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces13 = address.Assign (devices.Get(12));
address.SetBase ("10.1.14.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces14 = address.Assign (devices.Get(13));
address.SetBase ("10.1.15.0", "255.255.255.0");
  Ipv4InterfaceContainer p2pInterfaces;
  p2pInterfaces15 = address.Assign (devices.Get(14));*/
 

NS_LOG_INFO ("Create Applications.");
  uint16_t port = 9;   // Discard port (RFC 863)
  UdpEchoServerHelper echoServer (port);
//echoServer.SetAttribute ("MaxPackets", UintegerValue (1));
  //echoServer.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
//  echoServer.SetAttribute ("PacketSize", UintegerValue (1));

  ApplicationContainer serverApps = echoServer.Install (c.Get (0));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (p2pInterfaces.GetAddress (14), port);
  //echoClient.SetAttribute ("MaxPackets", UintegerValue (1));
 // echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
 // echoClient.SetAttribute ("PacketSize", UintegerValue (1));

  ApplicationContainer clientApps = 
    echoClient.Install (c.Get (14));
  //clientApps.Start (Seconds (1.0));
  //clientApps.Stop (Seconds (100.0));


  
 Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (1));
         Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (3));
Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (10));
         Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (11));
Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (12));
         Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (13));
Simulator::Schedule (Seconds (1.5), &Experiment::AdvancePosition, this, c.Get (14));
         
  Ptr<Socket> recvSink = SetupPacketReceive (c.Get (1));

  FlowMonitorHelper flowmon; 
  Ptr<FlowMonitor> monitor = flowmon.InstallAll (); 
monitor->CheckForLostPackets (); 
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> 
(flowmon.GetClassifier ()); 
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats (); 
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin (); i != stats.end (); ++i) 
    { 
      Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (i->first); 
      std::cout << "Flow " << (i->first) - 2 << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";          
 std::cout << " Tx Bytes:   " << i->second.txBytes << "\n"; 
      std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n"; 
      std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 8.0 / 1024 / 1024  << " Mbps\n"; 
      std::cout << "  Tx Packets:   " << i->second.txPackets << "\n"; 
      std::cout << "  Rx Packets:   " << i->second.rxPackets << "\n"; 
      std::cout << "  Delay Sum:   " << i->second.delaySum << "\n"; 
      std::cout << "  Average Delay:   " << i->second.delaySum / i->second.rxPackets<< "\n"; 
 
    } 

AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> osw = ascii.CreateFileStream ("aodvadd1.tr");
  phy.EnableAsciiAll (osw);

phy.EnablePcap ("aodvadd", devices.Get (0));
  Simulator::Run ();

  Simulator::Destroy ();

  return m_output;
}

int main (int argc, char *argv[])
{
  // disable fragmentation
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));

 std::string phyMode ("OfdmRate6MbpsBW10MHz");
 // uint32_t packetSize = 1000; // bytes
  //uint32_t numPackets = 1;
  //double interval = 1.0; // seconds
  bool verbose = false;
  CommandLine cmd;
  cmd.Parse (argc, argv);

  Gnuplot gnuplot = Gnuplot ("reference-rates.png");

  Experiment experiment;
  //WifiHelper wifi = WifiHelper::Default ();
  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
wifiChannel.AddPropagationLoss("ns3::NakagamiPropagationLossModel");
  Ptr<YansWifiChannel> channel = wifiChannel.Create ();
  wifiPhy.SetChannel (channel);
  // ns-3 supports generate a pcap trace
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11);

  NqosWaveMacHelper wifiMac = NqosWaveMacHelper::Default ();
  Wifi80211pHelper wifi = Wifi80211pHelper::Default ();

  if (verbose)
    {
      wifi.EnableLogComponents ();      // Turn on all Wifi 802.11p logging
    }
//wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
  //                                    "DataMode",StringValue (phyMode),
   //                                   "ControlMode",StringValue (phyMode));
  //wifi.SetStandard (WIFI_PHY_STANDARD_80211a);
  //NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  //YansWavePhyHelper wifiPhy =  YansWavePhyHelper::Default ();
   //YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
  //Ptr<YansWifiChannel> channel = wifiChannel.Create ();
 // wifiPhy.SetChannel (channel);
/*YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default ();
wifiPhy.SetChannel (wifiChannel.Create ());
//NqosWave80211pMacHelper wifiMac = NqosWaveMacHelper::Default();
QosWaveMacHelper wifiMac = QosWaveMacHelper::Default ();
 //wifiMac.SetType ("ns3::OcbWifiMac", "QosSupported", BooleanValue (true));
Wifi80211pHelper wifi = Wifi80211pHelper::Default ();
//WaveHelper wifi = WaveHelper::Default ();
 wifi.SetStandard (WIFI_PHY_STANDARD_80211_10MHZ);*/
   
	
  Gnuplot2dDataset dataset;

  //wifiMac.SetType ("ns3::AdhocWifiMac");

  NS_LOG_DEBUG ("54");
  experiment = Experiment ("54mb");
   wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate54Mbps"));//BW10MHz"));
                                     //"ControlMode",StringValue ("OfdmRate6MbpsBW10MHz"),
                                     //"NonUnicastMode", StringValue ("OfdmRate6MbpsBW10MHz")
                                    
  //wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
   //                             "DataMode", StringValue ("OfdmRate54Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("48");
  experiment = Experiment ("48mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate48Mbps"));//BW10MHz"));
                                     //"ControlMode",StringValue ("OfdmRate6MbpsBW10MHz"),MbpsBW10MHz"));
 // wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
   //                             "DataMode", StringValue ("OfdmRate48Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("36");
  experiment = Experiment ("36mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate36Mbps"));//bpsBW10MHz"));
 // wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
   //                             "DataMode", StringValue ("OfdmRate36Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("24");
  experiment = Experiment ("24mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate24Mbps"));//BW10MHz"));
 // wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
   //                             "DataMode", StringValue ("OfdmRate24Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("18");
  experiment = Experiment ("18mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate18Mbps"));//MbpsBW10MHz"));
 // wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
   //                             "DataMode", StringValue ("OfdmRate18Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("12");
  experiment = Experiment ("12mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate12Mbps"));//BW10MHz"));
//  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
  //                              "DataMode", StringValue ("OfdmRate12Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("9");
  experiment = Experiment ("9mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate9Mbps"));//BW10MHz"));
  //wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
       //                         "DataMode", StringValue ("OfdmRate9Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("6");
  experiment = Experiment ("0.6mb");
wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                     "DataMode", StringValue ("OfdmRate6Mbps"));//BW10MHz"));
//  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
 //                               "DataMode", StringValue ("OfdmRate6Mbps"));
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);


  gnuplot = Gnuplot ("rate-control.png");
  //wifi.SetStandard (WIFI_PHY_STANDARD_holland);


  NS_LOG_DEBUG ("arf");
  experiment = Experiment ("arf");
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("aarf");
  experiment = Experiment ("aarf");
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("aarf-cd");
  experiment = Experiment ("aarf-cd");
  wifi.SetRemoteStationManager ("ns3::AarfcdWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("cara");
  experiment = Experiment ("cara");
  wifi.SetRemoteStationManager ("ns3::CaraWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("rraa");
  experiment = Experiment ("rraa");
  wifi.SetRemoteStationManager ("ns3::RraaWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  NS_LOG_DEBUG ("ideal");
  experiment = Experiment ("ideal");
  wifi.SetRemoteStationManager ("ns3::IdealWifiManager");
  dataset = experiment.Run (wifi, wifiPhy, wifiMac, wifiChannel);
  gnuplot.AddDataset (dataset);

  gnuplot.GenerateOutput (std::cout);

  return 0;
}

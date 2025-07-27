 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example03 03 - using plugins
00004 ///
00005 /// fastjet plugins example program:
00006 ///   we illustrate the plugin usage
00007 ///   here, we use the SISCone plugin though different choices are possible
00008 ///   see the output of 'fastjet-config --list-plugins' for more details
00009 ///
00010 /// Note that when using plugins, the code needs to be linked against
00011 /// the libfastjetplugins library (with the default monolithic
00012 /// build. For non-monolithic build, individual libraries have to be
00013 /// used for each plugin). 
00014 /// This is ensured in practice by calling
00015 ///   fastjet-config --libs --plugins
00016 ///
00017 /// run it with    : ./03-plugin < data/single-event.dat
00018 ///
00019 /// Source code: 03-plugin.cc
00020 //----------------------------------------------------------------------
00021 
00022 //STARTHEADER
00023 // $Id: 03-plugin.cc 2684 2011-11-14 07:41:44Z soyez $
00024 //
00025 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00026 //
00027 //----------------------------------------------------------------------
00028 // This file is part of FastJet.
00029 //
00030 //  FastJet is free software; you can redistribute it and/or modify
00031 //  it under the terms of the GNU General Public License as published by
00032 //  the Free Software Foundation; either version 2 of the License, or
00033 //  (at your option) any later version.
00034 //
00035 //  The algorithms that underlie FastJet have required considerable
00036 //  development and are described in hep-ph/0512210. If you use
00037 //  FastJet as part of work towards a scientific publication, please
00038 //  include a citation to the FastJet paper.
00039 //
00040 //  FastJet is distributed in the hope that it will be useful,
00041 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00042 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00043 //  GNU General Public License for more details.
00044 //
00045 //  You should have received a copy of the GNU General Public License
00046 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00047 //----------------------------------------------------------------------
00048 //ENDHEADER
00049 
00050 #include "fastjet/ClusterSequence.hh"
00051 #include <iostream> // needed for io
00052 #include <cstdio>   // needed for io
00053 
00054 // include the SISCone plugin header if enabled
00055 #include "fastjet/config.h"
00056 #ifdef FASTJET_ENABLE_PLUGIN_SISCONE
00057 #include "fastjet/SISConePlugin.hh"
00058 #else
00059 #warning "SISCone plugin not enabled. Skipping the example"
00060 #endif // FASTJET_ENABLE_PLUGIN_SISCONE
00061 
00062 
00063 using namespace std;
00064 
00065 int main(){
00066 
00067 #ifdef FASTJET_ENABLE_PLUGIN_SISCONE
00068 
00069   // read in input particles
00070   //----------------------------------------------------------
00071   vector<fastjet::PseudoJet> input_particles;
00072   
00073   double px, py , pz, E;
00074   while (cin >> px >> py >> pz >> E) {
00075     // create a fastjet::PseudoJet with these components and put it onto
00076     // back of the input_particles vector
00077     input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
00078   }
00079   
00080 
00081   // create a jet definition fron a plugin.
00082   // It basically requires declaring a JetDefinition from a pointer to
00083   // the plugin
00084   //
00085   // we will use the SISCone plugin here. Its (mandatory) parameters
00086   // are a cone radius and an overlap threshold, plus other optional
00087   // parameters
00088   //
00089   // for other plugin, see individual documentations for a description
00090   // of their parameters
00091   //
00092   // the list of available plugins for a given build of FastJet can be
00093   // obtained using
00094   //   fastjet-config --list-plugins 
00095   // from the command line.
00096   //----------------------------------------------------------
00097   double cone_radius = 0.7;
00098   double overlap_threshold = 0.75;
00099   fastjet::SISConePlugin siscone(cone_radius, overlap_threshold);
00100   fastjet::JetDefinition jet_def(& siscone);
00101 
00102 
00103   // run the jet clustering with the above jet definition
00104   //----------------------------------------------------------
00105   fastjet::ClusterSequence clust_seq(input_particles, jet_def);
00106 
00107 
00108   // get the resulting jets ordered in pt
00109   //----------------------------------------------------------
00110   double ptmin = 5.0;
00111   vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00112 
00113 
00114   // tell the user what was done
00115   //  - the description of the algorithm used
00116   //  - extract the inclusive jets with pt > 5 GeV
00117   //    show the output as 
00118   //      {index, rap, phi, pt}
00119   //----------------------------------------------------------
00120   cout << "Ran " << jet_def.description() << endl;
00121 
00122   // label the columns
00123   printf("%5s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt");
00124  
00125   // print out the details for each jet
00126   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00127     printf("%5u %15.8f %15.8f %15.8f\n",
00128            i, inclusive_jets[i].rap(), inclusive_jets[i].phi(),
00129            inclusive_jets[i].perp());
00130   }
00131 
00132 #endif
00133 
00134   return 0;
00135 
00136 }
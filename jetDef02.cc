00001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example02 02 - changing the jet definition
00004 ///
00005 /// fastjet basic example program:
00006 ///   illustration of the usage of how to change the jet definition
00007 ///   used for the clustering (see also fastjet::JetDefinition)
00008 ///
00009 /// run it with    : ./02-jetdef < data/single-event.dat
00010 ///
00011 /// Source code: 02-jetdef.cc
00012 //----------------------------------------------------------------------
00013 //
00014 //STARTHEADER
00015 // $Id: 02-jetdef.cc 2684 2011-11-14 07:41:44Z soyez $
00016 //
00017 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00018 //
00019 //----------------------------------------------------------------------
00020 // This file is part of FastJet.
00021 //
00022 //  FastJet is free software; you can redistribute it and/or modify
00023 //  it under the terms of the GNU General Public License as published by
00024 //  the Free Software Foundation; either version 2 of the License, or
00025 //  (at your option) any later version.
00026 //
00027 //  The algorithms that underlie FastJet have required considerable
00028 //  development and are described in hep-ph/0512210. If you use
00029 //  FastJet as part of work towards a scientific publication, please
00030 //  include a citation to the FastJet paper.
00031 //
00032 //  FastJet is distributed in the hope that it will be useful,
00033 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00034 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00035 //  GNU General Public License for more details.
00036 //
00037 //  You should have received a copy of the GNU General Public License
00038 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00039 //----------------------------------------------------------------------
00040 //ENDHEADER
00041 
00042 
00043 #include "fastjet/ClusterSequence.hh"
00044 #include <iostream> // needed for io
00045 #include <cstdio>   // needed for io
00046 
00047 using namespace std;
00048 
00049 /// an example program showing how to use fastjet
00050 int main(){
00051   
00052   // read in input particles
00053   //----------------------------------------------------------
00054   vector<fastjet::PseudoJet> input_particles;
00055   
00056   double px, py , pz, E;
00057   while (cin >> px >> py >> pz >> E) {
00058     // create a fastjet::PseudoJet with these components and put it onto
00059     // back of the input_particles vector
00060     input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
00061   }
00062   
00063 
00064   // create a jet definition: 
00065   // a jet algorithm with a given radius parameter
00066   //----------------------------------------------------------
00067 
00068   // select a jet algorithm to use
00069   //
00070   // this could be one of
00071   //   {kt_algorithm, cambridge_algorithm, antikt_algorithm,
00072   //    genkt_algorithm, ee_kt_algorithm, ee_genkt_algorithm}
00073   // see example 03-plugin.cc for extra options using plugins
00074   // instead of naive algorithms)
00075   fastjet::JetAlgorithm jet_alg = fastjet::kt_algorithm;
00076 
00077   // when appropriate select a radius to use 
00078   //
00079   // this would not be mandatory for e^+ e^- algorithms (see 05-eplus_eminus.cc)
00080   double R = 0.6;
00081 
00082   // select an __optional__ recombination scheme
00083   //
00084   // this could be one of
00085   //   {E_scheme, pt_scheme, pt2_scheme, Et_scheme, Et2_scheme, BIpt_scheme, 
00086   //    BIpt2_scheme, external_sheme}
00087   // 
00088   // Notes:
00089   //  - for the usage of a user-defined recombination scheme
00090   //    (external_scheme), see 11-boosted_higgs.cc
00091   //
00092   // By default, the E_scheme is used 
00093   fastjet::RecombinationScheme recomb_scheme=fastjet::E_scheme;
00094 
00095   // select an __optional__ strategy
00096   //
00097   // this could be chosen among
00098   //   {N2MinHeapTiled, N2Tiled, N2PoorTiled, N2Plain, N3Dumb, 
00099   //    Best, 
00100   //    NlnN, NlnN3pi, NlnN4pi, NlnNCam4pi, NlnNCam2pi2R, NlnNCam}
00101   //
00102   // By default, the Best strategy is chosen and we advise to keep
00103   // that default unless you are targeting a very specific usage. Note
00104   // also that the N log (N) strategies for algorithms other than
00105   // Cambridge/Aachen need CGAL support.
00106   fastjet::Strategy strategy = fastjet::Best;
00107 
00108   // create the JetDefinition from the above information
00109   fastjet::JetDefinition jet_def(jet_alg, R, recomb_scheme, strategy);
00110 
00111 
00112   // run the jet clustering with the above jet definition
00113   //----------------------------------------------------------
00114   fastjet::ClusterSequence clust_seq(input_particles, jet_def);
00115 
00116 
00117   // get the resulting jets ordered in pt
00118   //----------------------------------------------------------
00119   double ptmin = 5.0;
00120   vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00121 
00122 
00123   // tell the user what was done
00124   //  - the description of the algorithm used
00125   //  - extract the inclusive jets with pt > 5 GeV
00126   //    show the output as 
00127   //      {index, rap, phi, pt}
00128   //----------------------------------------------------------
00129   cout << "Ran " << jet_def.description() << endl;
00130 
00131   // label the columns
00132   printf("%5s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt");
00133  
00134   // print out the details for each jet
00135   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00136     printf("%5u %15.8f %15.8f %15.8f\n",
00137            i, inclusive_jets[i].rap(), inclusive_jets[i].phi(),
00138            inclusive_jets[i].perp());
00139   }
00140 
00141   return 0;
00142 }
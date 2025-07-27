//----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example01 01 - basic usage example
00004 ///
00005 /// fastjet basic example program:
00006 ///   simplest illustration of the usage of the basic classes:
00007 ///   fastjet::PseudoJet, fastjet::JetDefinition and 
00008 ///   fastjet::ClusterSequence
00009 ///
00010 /// run it with    : ./01-basic < data/single-event.dat
00011 ///
00012 /// Source code: 01-basic.cc
00013 //----------------------------------------------------------------------
00014 
00015 //STARTHEADER
00016 // $Id: 01-basic.cc 2684 2011-11-14 07:41:44Z soyez $
00017 //
00018 // Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
00019 //
00020 //----------------------------------------------------------------------
00021 // This file is part of FastJet.
00022 //
00023 //  FastJet is free software; you can redistribute it and/or modify
00024 //  it under the terms of the GNU General Public License as published by
00025 //  the Free Software Foundation; either version 2 of the License, or
00026 //  (at your option) any later version.
00027 //
00028 //  The algorithms that underlie FastJet have required considerable
00029 //  development and are described in hep-ph/0512210. If you use
00030 //  FastJet as part of work towards a scientific publication, please
00031 //  include a citation to the FastJet paper.
00032 //
00033 //  FastJet is distributed in the hope that it will be useful,
00034 //  but WITHOUT ANY WARRANTY; without even the implied warranty of
00035 //  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
00036 //  GNU General Public License for more details.
00037 //
00038 //  You should have received a copy of the GNU General Public License
00039 //  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
00040 //----------------------------------------------------------------------
00041 //ENDHEADER
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
00067   double R = 0.6;
00068   fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
00069 
00070 
00071   // run the jet clustering with the above jet definition
00072   //----------------------------------------------------------
00073   fastjet::ClusterSequence clust_seq(input_particles, jet_def);
00074 
00075 
00076   // get the resulting jets ordered in pt
00077   //----------------------------------------------------------
00078   double ptmin = 5.0;
00079   vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00080 
00081 
00082   // tell the user what was done
00083   //  - the description of the algorithm used
00084   //  - extract the inclusive jets with pt > 5 GeV
00085   //    show the output as 
00086   //      {index, rap, phi, pt}
00087   //----------------------------------------------------------
00088   cout << "Ran " << jet_def.description() << endl;
00089 
00090   // label the columns
00091   printf("%5s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt");
00092  
00093   // print out the details for each jet
00094   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00095     printf("%5u %15.8f %15.8f %15.8f\n",
00096            i, inclusive_jets[i].rap(), inclusive_jets[i].phi(),
00097            inclusive_jets[i].perp());
00098   }
00099 
00100   return 0;
00101 }
001 //----------------------------------------------------------------------
00002 /// \file
00003 /// \page Example06 06 - using jet areas
00004 ///
00005 /// fastjet example program for jet areas
00006 /// It mostly illustrates the usage of the 
00007 /// fastjet::AreaDefinition and fastjet::ClusterSequenceArea classes
00008 ///
00009 /// run it with    : ./06-area < data/single-event.dat
00010 ///
00011 /// Source code: 06-area.cc
00012 //----------------------------------------------------------------------
00013 
00014 //STARTHEADER
00015 // $Id: 06-area.cc 2684 2011-11-14 07:41:44Z soyez $
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
00042 #include "fastjet/ClusterSequenceArea.hh"  // use this instead of the "usual" ClusterSequence to get area support
00043 #include <iostream> // needed for io
00044 #include <cstdio>   // needed for io
00045 
00046 using namespace std;
00047 
00048 /// an example program showing how to use fastjet
00049 int main(){
00050   
00051   // read in input particles
00052   //----------------------------------------------------------
00053   vector<fastjet::PseudoJet> input_particles;
00054   
00055   double px, py , pz, E;
00056   while (cin >> px >> py >> pz >> E) {
00057     // create a fastjet::PseudoJet with these components and put it onto
00058     // back of the input_particles vector
00059     input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
00060   }
00061   
00062 
00063   // create a jet definition: 
00064   // a jet algorithm with a given radius parameter
00065   //----------------------------------------------------------
00066   double R = 0.6;
00067   fastjet::JetDefinition jet_def(fastjet::kt_algorithm, R);
00068 
00069 
00070   // Now we also need an AreaDefinition to define the properties of the 
00071   // area we want
00072   //
00073   // This is made of 2 building blocks:
00074   //  - the area type:
00075   //    passive, active, active with explicit ghosts, or Voronoi area
00076   //  - the specifications:
00077   //    a VoronoiSpec or a GhostedAreaSpec for the 3 ghost-bases ones
00078   // 
00079   //---------------------------------------------------------- For
00080   // GhostedAreaSpec (as below), the minimal info you have to provide
00081   // is up to what rapidity ghosts are placed. 
00082   // Other commonm parameters (that mostly have an impact on the
00083   // precision on the area) include the number of repetitions
00084   // (i.e. the number of different sets of ghosts that are used) and
00085   // the ghost density (controlled through the ghost_area).
00086   // Other, more exotic, parameters (not shown here) control how ghosts
00087   // are placed.
00088   //
00089   // The ghost rapidity interval should be large enough to cover the
00090   // jets for which you want to calculate. E.g. if you want to
00091   // calculate the area of jets up to |y|=4, you need to put ghosts up
00092   // to at least 4+R (or, optionally, up to the largest particle
00093   // rapidity if this is smaller).
00094   double maxrap = 5.0;
00095   unsigned int n_repeat = 3; // default is 1
00096   double ghost_area = 0.01; // this is the default
00097   fastjet::GhostedAreaSpec area_spec(maxrap, n_repeat, ghost_area);
00098 
00099   fastjet::AreaDefinition area_def(fastjet::active_area, area_spec);
00100 
00101   // run the jet clustering with the above jet and area definitions
00102   //
00103   // The only change is the usage of a ClusterSequenceArea rather than
00104   //a ClusterSequence
00105   //----------------------------------------------------------
00106   fastjet::ClusterSequenceArea clust_seq(input_particles, jet_def, area_def);
00107 
00108 
00109   // get the resulting jets ordered in pt
00110   //----------------------------------------------------------
00111   double ptmin = 5.0;
00112   vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));
00113 
00114 
00115   // tell the user what was done
00116   //  - the description of the algorithm and area used
00117   //  - extract the inclusive jets with pt > 5 GeV
00118   //    show the output as 
00119   //      {index, rap, phi, pt, number of constituents}
00120   //----------------------------------------------------------
00121   cout << endl;
00122   cout << "Ran " << jet_def.description() << endl;
00123   cout << "Area: " << area_def.description() << endl << endl;
00124 
00125   // label the columns
00126   printf("%5s %15s %15s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt", "area", "area error");
00127  
00128   // print out the details for each jet
00129   for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
00130     printf("%5u %15.8f %15.8f %15.8f %15.8f %15.8f\n", i,
00131            inclusive_jets[i].rap(), inclusive_jets[i].phi(), inclusive_jets[i].perp(),
00132            inclusive_jets[i].area(), inclusive_jets[i].area_error());
00133   }
00134 
00135   return 0;
00136 }
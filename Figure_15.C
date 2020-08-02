#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include "TROOT.h"
#include "TStyle.h"



//Interpolation Algorithm
double interpolation(vector<double> x, vector<double> curve_1, int row_total, double x_interp)
{
  //Need to find the two points nearest the interpolating region
  double temp, lower_index, higher_index;
  for (int i = 0; i < row_total; i++)
    {
      temp =  x_interp - x[i];
      if (temp <= 0)
	{
	  lower_index = i-1; //index of the number that is smaller than our x_interp
	  higher_index = i; //index of the number that is larger than our x_interp
	  break; //we will be using these two numbers to interpolate the middle number which is x_interp
	}
    }
  //Use linear interpolation to make a construct a straight line using point slope form.
  double m = (curve_1[higher_index] - curve_1[lower_index] ) / ( x[higher_index] - x[lower_index]); //calcuating the slope from lower and higher number
  double y = curve_1[lower_index] - m * ( x[lower_index] - x_interp); // calcuating the y value using point slope form
  return y; // return the interpolated y
}



void Figure_15()
{ 
  
  //Declaring the variables we will be using for storying the data
  vector<double> x, curve_1;

  //Declaring the temporary variables we will be using to scan through the data.
  string col0, col1, line;

  //Load the csv
  ifstream figure_15("Figure15.csv");

  getline(figure_15,line); //get rid of the first row

  int row_total = 0; //variable used to count number of rows

  //Begin reading sequence
  while (!figure_15.eof()) //careful, eof for some reason reads 1 extra column after the file has ended, the if statement is used to correct that.
    {
      getline(figure_15,col0, ',');
      if (col0 == ""){
	break;}//prevent eof from reading one extra line at the end
      x.push_back(stof(col0));

      getline(figure_15,col1, '\n');
      curve_1.push_back(stof(col1));

      row_total += 1;
    }
  
  //DELCARE MOEDAL STYLE
  TStyle *MoEDALStyle = new TStyle("MoEDAL", "MoEDAL style");
  Int_t icol=0; // WHITE
  MoEDALStyle->SetFrameBorderMode(icol);
  MoEDALStyle->SetFrameFillColor(icol);
  MoEDALStyle->SetCanvasBorderMode(icol);
  MoEDALStyle->SetCanvasColor(icol);
  MoEDALStyle->SetPadBorderMode(icol);
  MoEDALStyle->SetPadColor(icol);
  MoEDALStyle->SetStatColor(icol);
  
  // set the paper & margin sizes
  MoEDALStyle->SetPaperSize(20,26);
  
  // set margin sizes
  MoEDALStyle->SetPadTopMargin(0.05);
  MoEDALStyle->SetPadRightMargin(0.05);
  MoEDALStyle->SetPadBottomMargin(0.16);
  MoEDALStyle->SetPadLeftMargin(0.16);
  
  // set title offsets (for axis label)
  MoEDALStyle->SetTitleXOffset(1.4);
  MoEDALStyle->SetTitleYOffset(1.4);
  
  // use large fonts
  Int_t font=42; // Helvetica
  Double_t tsize=0.059;
  MoEDALStyle->SetTextFont(font);
  
  MoEDALStyle->SetTextSize(tsize);
  MoEDALStyle->SetLabelFont(font,"x");
  MoEDALStyle->SetTitleFont(font,"x");
  MoEDALStyle->SetLabelFont(font,"y");
  MoEDALStyle->SetTitleFont(font,"y");
  MoEDALStyle->SetLabelFont(font,"z");
  MoEDALStyle->SetTitleFont(font,"z");
  
  MoEDALStyle->SetLabelSize(tsize,"x");
  MoEDALStyle->SetTitleSize(tsize,"x");
  MoEDALStyle->SetLabelSize(tsize,"y");
  MoEDALStyle->SetTitleSize(tsize,"y");
  MoEDALStyle->SetLabelSize(tsize,"z");
  MoEDALStyle->SetTitleSize(tsize,"z");
  
  // use bold lines and markers
  MoEDALStyle->SetMarkerStyle(20);
  MoEDALStyle->SetMarkerSize(1.2);
  MoEDALStyle->SetHistLineWidth(2.);
  MoEDALStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  
  MoEDALStyle->SetEndErrorSize(0.);
  
  // do not display any of the standard histogram decorations
  MoEDALStyle->SetOptTitle(1);
  MoEDALStyle->SetTitleFillColor(0);
  MoEDALStyle->SetTitleBorderSize(0);
  MoEDALStyle->SetTitleFontSize(0.045);
  MoEDALStyle->SetTitleX(0.16);
  MoEDALStyle->SetTitleY(1.0);
  MoEDALStyle->SetOptStat(0);
  MoEDALStyle->SetOptFit(0);
  
  // put tick marks on top and RHS of plots
  MoEDALStyle->SetPadTickX(1);
  MoEDALStyle->SetPadTickY(1);
  MoEDALStyle -> cd();
  
  gROOT -> SetStyle("MoEDAL");
  gROOT -> ForceStyle();


  auto c1 = new TCanvas("c1","c1", 1000,700);

  TGraph* g = new TGraph();
  for (int i = 0; i<row_total; i++)
    {
      g -> SetPoint(i,x[i],curve_1[i]);
    }
  //g -> SetLineWidth(4);
  //g -> SetLineColor(kAzure+9);
  g -> SetMarkerSize(0.4);
  g -> SetMarkerColor(kAzure+9);
    
  g -> GetXaxis() -> SetTitle("m_{S} [GeV]");
  g -> GetYaxis() -> SetTitle("#tau_{S} [sec]");
  g -> GetYaxis() -> SetRangeUser(pow(10,-9.3),pow(10,2));
  g -> GetXaxis() -> SetLimits(0.1 , 50); //X WINDOW SETTINGS

  g->Draw("AP");

  c1 -> SetLogy();
  c1 -> SetLogx();

  //c1 -> SaveAs("Figure_15_Patrick.png");

  //testing the interpolated values and plottign them over top of the original graph
  auto *g1 = new TGraph();
  for (int i = 0; i < row_total; i++)
    g1 -> SetPoint(i,x[i], interpolation(x,curve_1,row_total,x[i]));

  g1 -> Draw("P");
  g1 -> SetMarkerSize(0.4);
  g1 -> SetMarkerColor(kRed+1);

  c1 -> SaveAs("Figure_15_Patrick.png");

}


  
  
   
  

      
      

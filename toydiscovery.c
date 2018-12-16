#include<TH1D.h>
#include<TROOT.h>
#include "TH1.h"
#include "TF1.h"


TList* rebindHist(const char* fileIn, Int_t rebindNum){
    // Function made by Anthony and Cynthia
    // Input:
    //        * Name of the file  (const char)
    //        * number of the rebining (Int_t)
    // Output:
    //        * List with all the histograms rebined (TList)

    // Read the root file
    TFile *f1 = new TFile(fileIn);
    // Create a iterator with the keys of the file
    TIter next(f1->GetListOfKeys());
    // Declare a Key that will help us to iterate
    TKey *key;
    // Declare a TList
    TList* histoList = new TList();
    // Declare a TCanvas
    TCanvas c1;
    // Start the interations
    while ((key = (TKey*)next())) {
        // Obtain the name of each key of the file
        TClass *cl = gROOT->GetClass(key->GetClassName());
        // If the class is TH1, then we read the object that will
        // be a histogram
        if (!cl->InheritsFrom("TH1")) continue;
        TH1 *h = (TH1*)key->ReadObj();
        // Create a new rebined histogram
        TH1F *hnew = dynamic_cast<TH1F*>(h->Rebin(rebindNum,"hnew"));
        // Concatenate the new rebined histogram to the TList
        histoList->Add(hnew);
    }
    // Return a TList with the histograms rebinned
    return histoList;
}



void integrateFits(TList* histoList){
// Carlos and Juan Daniel
// Function that recive a TList with the histograms
  FILE* file = fopen("Events","w"); // Open a file in which we will print the Events
  TIter liter(histoList); // Create a iterator of the TList
  TObject *obj = 0; // Create a object that will help to iterate
  Int_t i = 1; // Create a number for the interations
  while((obj = liter.Next())!=0){
    fprintf(file, "Histogram %d\n\n",i);
    fprintf(file, "------------------------------------------------\n\n");
    TH1 *h1 = (TH1*)obj;
    TF1 *func = (TF1*)h1->GetFunction("sig");
    // Obtain the function that fit the signal
    TF1 *func1 = (TF1*)h1->GetFunction("back");
    // Obtain the function that fit the background
    Double_t inte = func->Integral(50,500);
    // Integrate the function of the signal
    Double_t inte1 = func1->Integral(0,800);
    // Integrate the function of the background
    // Print the results on the file
    fprintf(file,"Nevents Sig = \t %f \n",inte );
    fprintf(file, "------------------------------------------------\n\n");
    fprintf(file,"Nevents back =\t %f \n",inte1 );
    i += 1;
  }
}

TH1* fitPeak(TH1* h){
// Iskra and Andrés:
// Functtion that take a histogram and return a fitted histogram
    // Declare a list in which we are going to store the parameters of the fits
    Double_t par[16];
    // Functions that will help us to fit the signal
    TF1 *g1    = new TF1("g1","gaus",70,102);
    TF1 *g2    = new TF1("g2","gaus",118,158);
    TF1 *g3    = new TF1("g3","gaus",181,185);
    TF1 *g4    = new TF1("g4","gaus",188,233);
    TF1 *ep    = new TF1("ep","landau",250,500);
    // Function for the background
    TF1 *back  = new TF1("back","pol2",0,800);
    // Function for the signal
    TF1 *sig = new TF1("sig","gaus(0)+gaus(3)+gaus(6)+gaus(9)+landau(12)",50,500);
    // Function for the full spectrum
    TF1 *full = new TF1("full","gaus(0)+gaus(3)+gaus(6)+gaus(9)+landau(12)+pol2(14)",30,800);
    // Set different collors to the signal and the background
    sig->SetLineColor(4); // Blue
    full->SetLineColor(5); // Yellow
    // Fit the functions (N allow us to not plot this fits)
    h->Fit(g1,"R","N");
    h->Fit(g2,"R+","N");
    h->Fit(g3,"R+","N");
    h->Fit(g4,"R+","N");
    h->Fit(ep,"R+","N");
    h->Fit(back,"R+","N");
    // Obtain the parameters of the fits
    g1->GetParameters(&par[0]);
    g2->GetParameters(&par[3]);
    g3->GetParameters(&par[6]);
    g4->GetParameters(&par[9]);
    ep->GetParameters(&par[12]);
    back->GetParameters(&par[14]);
    // Set this parameters to the signal and background
    sig->SetParameters(par);
    full->SetParameters(par);
    // Fit the signal the background and the full spectrum
    // (L fit with likelihood)
    h->Fit(sig,"R","L");
    h->Fit(back,"R+","L");
    h->Fit(full,"R+","L");
    // Create a neww histogram
    TH1 *hisSignal = (TH1*) h;
    // Create a neww histogram with the signal minus the background
    hisSignal->Add(back,-1);

    // Return the histogram fitted
    return hisSignal;
}

TList* fitPeaksList(TList* histoList){
// Iskra and Andrés:
// Function that take a TList of histograms and fit each one
    // Create a iterator
    TIter liter(histoList);
    // Auxiliar TList
    TList* auxList = new TList();
    // Object to iterate
    TObject *obj = 0;
    while((obj = liter.Next())!=0){
        // Take each histogram of the TList
        TH1 *h1 = (TH1*)obj;
        // Fit the histogram
        TH1* fit = fitPeak(h1);
        // Save the fitted histogram in the Auxiliar list
        auxList->Add(fit);
    }
    // return the TList with fitted histograms
    return auxList;
}

void showParameterFits(TList* histoList){
// Iskra and Andrés:
// Function that take a TList and print into a file all the statistical
// parameters of the fit
  // Declare a file
  FILE* parameters;
  // Create a iterator
  TIter liter(histoList);
  // Open a file
  parameters = fopen("FitsParameters","w");
  // create a object to iterate
  TObject *obj = 0;
  // Create a int that count the iterations
  Int_t i = 1;

  while((obj = liter.Next())!=0){
      fprintf(parameters, "Histogram %d\n\n",i);
      fprintf(parameters, "---------------------------------------------\n");
      // Obtain the histogram of the TList
      TH1 *h1 = (TH1*)obj;
      // Get the statistical parameters and print them on the file
      TF1 *func = (TF1*)h1->GetFunction("full");
      Double_t chi = func->GetChisquare();
      Double_t ndf = func->GetNDF();
      Double_t prob = func->GetProb();
      fprintf(parameters,"Chi Square =%f \n",chi );
      fprintf(parameters,"NDF =%f \n",ndf);
      fprintf(parameters, "---------------------------------------------\n");
      fprintf(parameters, "\n");
      i += 1;
  }
}



TList* cumulativeList(TList* hists){
// Juan David and Genesis Code
// Function that take a TList with rebinned histograms and create the cummulative
// histograms
    // Create a iterator
    TIter liter(hists);
    // Create an auxiliar list
    TList* auxList = new TList();
    // Create an object to iterate
    TObject *obj = 0;
    while((obj = liter.Next())!=0){
        // Take the histograms of the List
        TH1 *h1 = (TH1*)obj;
        // Create a new histogram with the cummulative
        TH1* hc = h1->GetCumulative();
        // Put the new histogram in the auxiliar list
        auxList->Add(hc);
        //cumulative(h1);
    }
    // return a TList with the cummulative histograms
    return auxList;
}

void showHist(TList* histList){
// Edwin Váscpnez and Truman Tapia Code 
// Function that take a TList with the histograms and show the plots on
// the screen
    // Define a char
    char histTittle[99];
    // Create a iterator
    TIter liter(histList);
    // Create a object to iterate
    TObject *obj = 0;
    Int_t i = 1;
    // Create a char to set the histograms names

    while((obj = liter.Next())!=0){
      sprintf(histTittle, "Histogram %i", i);
      // Create a canvas
        TCanvas* c = new TCanvas;
      // Take a histogram from the TList
        TH1 *h1 = (TH1*)obj;
      // Draw thw histogram
        h1->Draw();
        h1->SetTitle(histTittle);
        h1->GetXaxis()->SetTitle("Events");
      // Set a color to the histogram
        h1->SetLineColor(1);
      // Update the canvas
        c->Update();
        i+=1;
    }
}


void toydiscovery(){
    TList* hists = rebindHist("toy_discovery.root",10);
    TList* cumulH = cumulativeList(hists);
    showHist(cumulH);
    showHist(hists);
    TList* fits = fitPeaksList(hists);
    showHist(fits);
    integrateFits(fits);
    showParameterFits(fits);

}

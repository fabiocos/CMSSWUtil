void testnoise() {

  TRandom * thisGene = new TRandom();

  TH1F * gaussian = new TH1F("gaussian","gaussian",200,190.,210.);
  TH1F * gaussian1 = new TH1F("gaussian1","gaussian1",200,190.,210.);
  TH1F * gaussian2 = new TH1F("gaussian2","gaussian2",200,190.,210.);

  Double_t mean = 198.8;
  Double_t rms = 1.1;
  rms1 = sqrt(pow(rms,2)-1./12.);
  std::cout << "True rms = " << rms1 << std::endl;
  Bool_t norm = kFALSE;
  for ( unsigned int i = 0; i<100000; ++i) {
    Double_t gausrand = thisGene->Gaus(mean,rms);
    gaussian->Fill(gausrand);
    gausrand = thisGene->Gaus(mean,rms1);
    int adc = (int)gausrand;
    Double_t dadc = (Double_t)adc; 
    cadc = Double_t (gausrand-adc <= 0.5 ? adc : adc+1); 
    //Double_t cadc = (Double_t) (TMath::Nint(gausrand));
    //std::cout << gausrand-adc << " " << adc << " " << cadc << std::endl;
    //std::cout << " Gaussian " << i << " " << gausrand << std::endl;
    gaussian1->Fill(dadc);
    gaussian2->Fill(cadc);
  }

  gStyle->SetOptFit(1111);

  gaussian->Fit("gaus","V");
  gaussian1->Fit("gaus","V");
  gaussian2->Fit("gaus","V");

  TCanvas * MyC = new TCanvas("MyC","Test canvas",1); 
  MyC->Divide(3,1);
  MyC->cd(1);
  gaussian->Draw(); 
  MyC->cd(2);
  gaussian1->Draw(); 
  MyC->cd(3);
  gaussian2->Draw(); 
  MyC->Print("gaussiana.eps");
  delete gaussian;
  delete MyC;

}

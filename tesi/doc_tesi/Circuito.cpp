//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Circuito.h"
#include "CircNum.h"

#pragma package(smart_init)
double Pulsaz[10];
int NumPuls;
//---------------------------------------------------------------------------
int __fastcall Compare(void* Item1, void* Item2)
{ Componente* c1=(Componente*)Item1;
  Componente* c2=(Componente*)Item2;
  double val1=c1->Val;
  double val2=c2->Val;
  if(c1->Nome[1]=='C') val1 *= Pulsaz[NumPuls-1];
  if(c2->Nome[1]=='C') val2 *= Pulsaz[NumPuls-1];
  if(c1->Nome[1]=='L') val1 = 1.0/(val1*Pulsaz[0]);
  if(c2->Nome[1]=='L') val2 = 1.0/(val2*Pulsaz[0]);
  if(c1->Nome[1]=='Y') val1 = 1.0/val1;
  if(c2->Nome[1]=='Y') val2 = 1.0/val2;
  if(c1->Nome[1]=='R') val1 = 1.0/val1;
  if(c2->Nome[1]=='R') val2 = 1.0/val2;
  if(val2 < val1) return -1;
  else if(val2 > val1) return +1;
  else return 0;
}
//---------------------------------------------------------------------------
int __fastcall CompAlfa(void* Item1, void* Item2)
{ Componente* c1=(Componente*)Item1;
  Componente* c2=(Componente*)Item2;
  if(c2->Symb && !c1->Symb) return +1;
  if(!c2->Symb && c1->Symb) return -1;
  if(c2->Nome < c1->Nome) return +1;
  else if(c2->Nome > c1->Nome) return -1;
  else return 0;
}
//---------------------------------------------------------------------------
__fastcall Circuito::Circuito(String file)
{
int i;
String riga,riga2;
char Buff[40];
Componente* comp;

Abort=false;
Approx=false;
FWarn=false;
FileName=file;
FileFdt=ChangeFileExt(FileName,".fdt");
FilePar=ChangeFileExt(FileName,".par");
Nodi=new TStringList();
TStringList* netlist=new TStringList();
netlist->LoadFromFile(FileName);
Compon=new TList();
OpAmp=new TList();
Impedenze=new TStringList();
extra=1;
NomeIng="";
NodoIng=AddNodo("$0");
NodoRef=-99;
IngVal=1;
IngSymb=false;
TipoIng='V';

for(i=1;i<netlist->Count;i++)
   { riga=netlist->Strings[i];
     if(!SetComp(riga))
            { ShowMessage("Error in line "+String(i));
              return;
            }
   }
delete netlist;
if(Approx)
  { Compon->Sort(Compare);
    SetImpedenze();
    CircNum* CircN=new CircNum(FileName);
    for(i=0;i<NumPuls;i++) ValNum[i] = CircN->Numerat(Pulsaz[i]);
    for(i=0;i<NumPuls;i++) ValDen[i] = CircN->Denom(Pulsaz[i]);
  }
else
  { Compon->Sort(CompAlfa);
    SetImpedenze();
  }

NNodi=Nodi->Count;
NBra=Compon->Count;

fp=fopen(FilePar.c_str(),"wb");
riga="";
riga2="";
IDx=new int[Compon->Count];

TStringList* nomi=new TStringList();
for(i=0;i<Compon->Count;i++)
  { comp=(Componente*)Compon->Items[i];
    if(nomi->IndexOf(comp->Nome)==-1)
       { nomi->Add(comp->Nome);
         if(comp->Symb) riga += comp->Nome+" ";
         else riga += "$ ";
         riga2 += String(gcvt(comp->Val,9,Buff))+" ";
       }
    IDx[i]=nomi->IndexOf(comp->Nome);
  }
delete nomi;
riga+="\r\n";
riga2+="\r\n";
fputs(riga.c_str(),fp);
fputs(riga2.c_str(),fp);
fclose(fp);
fp=fopen(FileFdt.c_str(),"wb");
}
//---------------------------------------------------------------------------
__fastcall Circuito::~Circuito()
{
int i;
if(fp) fclose(fp);
for(i=0;i<Compon->Count;i++)
  { Componente* c=(Componente*)Compon->Items[i];
    delete c;
  }
delete Compon;
for(i=0;i<OpAmp->Count;i++)
  { Componente* c=(Componente*)OpAmp->Items[i];
    delete c;
  }
delete OpAmp;
delete Impedenze;
delete Nodi;
delete[] IDx;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::ErrorMsg(String msg)
{
Application->MessageBox(msg.c_str(),"Error",MB_OK | MB_ICONHAND);
ExitProcess(0);
}
//---------------------------------------------------------------------------
void __fastcall Circuito::SetImpedenze()
{
Componente* c;
Impedenze->Clear();
for(int i=0;i<Compon->Count;i++)
  { c=(Componente*)Compon->Items[i];
    if(c->Imped) Impedenze->Add(String(i));
  }
}
//---------------------------------------------------------------------------
int __fastcall Circuito::AddNodo(String nn)
{
int n=Nodi->IndexOf(nn);
if(n!=-1) return n+1;
Nodi->Add(nn);
return Nodi->IndexOf(nn)+1;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Passivo(String nome, int n1, int n2, double val, bool symb)
{
 Componente* comp=new Componente();
 comp->Nome=nome;
 comp->Nv[0]=comp->Ni[0]=n1;
 comp->Nv[1]=comp->Ni[1]=n2;
 comp->Val=val;
 comp->Symb=symb;
 comp->Reattivo=(nome[1] == 'C' || nome[1] == 'L');
 comp->Imped=(nome[1] == 'L' || nome[1] == 'R');
 Compon->Add(comp);
}
//---------------------------------------------------------------------------
void __fastcall Circuito::VCCS(String nome, int n1, int n2, int n3, int n4, double val, bool symb)
{
 Componente* comp=new Componente();
 comp->Nome=nome;
 comp->Ni[0]=n1;
 comp->Ni[1]=n2;
 comp->Nv[0]=n3;
 comp->Nv[1]=n4;
 comp->Val=val;
 comp->Symb=symb;
 comp->Attivo=true;
 if(nome[1]=='Y' || nome[1]=='K') comp->Imped=true;
 if(nome[1]=='K') comp->Reattivo=true;
 Compon->Add(comp);
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Opamp(int n1, int n2, int n3, int n4)
{
 Componente* comp=new Componente();
 comp->Nome=" ";
 comp->Ni[1]=n1;
 comp->Ni[0]=n2;
 comp->Nv[1]=n3;
 comp->Nv[0]=n4;
 comp->Attivo=true;
 OpAmp->Add(comp);
}
//---------------------------------------------------------------------------
bool __fastcall Circuito::SetComp(String riga)
{
if(Trim(riga)=="") return true;
riga=Trim(riga);
char first=(riga.UpperCase())[1];
if(first=='*') return true;
LPSTR ss;
String nome,cmd;
double val;
bool symb;
int n1,n2,n3,n4,n5,n6,n7;

switch(first)
  { case 'V':
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         if(NodoRef==-99) NodoRef=n2;
         n3=AddNodo("$0");
         n4=NodoRef;
         n5=AddNodo("$"+String(extra++));
         ss=strtok(NULL," ");
         val=strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         VCCS(nome,n5,n2,n3,n4,val,symb);
         Opamp(n1,n2,n5,n2);
         VCCS(" ",n5,n2,n1,n2,-1.0,false);
         return true;
    case 'I':
         nome=strtok(riga.c_str()," ");
         n2=AddNodo(strtok(NULL," "));
         n1=AddNodo(strtok(NULL," "));
         if(NodoRef==-99) NodoRef=n1;
         n3=AddNodo("$0");
         n4=NodoRef;
         ss=strtok(NULL," ");
         val=strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         VCCS(nome,n1,n2,n3,n4,val,symb);
         return true;
    case '.':
         cmd=strtok(riga.c_str()," ");
         cmd=cmd.UpperCase();
         if(cmd==".OUT")
           { NodOut1=AddNodo(strtok(NULL," "));
             NodOut2=AddNodo("0");
           }
         else if(cmd==".IOUT")
           { NodOut1=AddNodo("$"+String(extra++));
             NodOut2=AddNodo("0");
             n1=AddNodo(strtok(NULL," "));
             n2=AddNodo(strtok(NULL," "));
             Opamp(NodOut1,NodOut2,n2,n1);
             VCCS(" ",n1,n2,NodOut1,NodOut2,1.0,false);
           }
         else if(cmd==".APP")
           { Limite=strtod(strtok(NULL," "),NULL)/100.0;
             NumPuls=atoi(strtok(NULL," "));
             for(n1=0;n1<NumPuls;n1++) Pulsaz[n1]=strtod(strtok(NULL," "),NULL);
             Approx=true;
           }
         return true;
    case 'G':
    case 'C':
    case 'L':
    case 'R':
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         ss=strtok(NULL," ");
         val=strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         Passivo(nome,n1,n2,val,symb);
         return true;
    case 'N':
//                      add_edge
//  comp->Ni[0]=n1;       1
//  comp->Ni[1]=n2;       2
//  comp->Nv[0]=n3;       3
//  comp->Nv[1]=n4;       4
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         n3=AddNodo(strtok(NULL," "));
         n4=AddNodo(strtok(NULL," "));
         n5=AddNodo("$"+String(extra++));
         n6=AddNodo("$"+String(extra++));
         n7=AddNodo("$"+String(extra++));
         ss=strtok(NULL," ");
         val=strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         VCCS(nome,n6,n2,n1,n2,val,symb);
         Opamp(n5,n4,n6,n4);
         VCCS(" ",n6,n4,n5,n4,-1.0,false);
         Opamp(n7,n2,n5,n3);
         VCCS(" ",n5,n3,n7,n2,1.0,false);
         VCCS(nome,n1,n2,n7,n2,val,symb);
         return true;
    case 'H':
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         n3=AddNodo(strtok(NULL," "));
         n4=AddNodo(strtok(NULL," "));
         ss=strtok(NULL," ");
         val=strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         VCCS(nome,n1,n2,n3,n4,val,symb);
         return true;
    case 'E': // VCVS
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         n3=AddNodo(strtok(NULL," "));
         n4=AddNodo(strtok(NULL," "));
         n5=AddNodo("$"+String(extra++));
         ss=strtok(NULL," ");
         val=strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         VCCS(nome,n5,n2,n3,n4,val,symb);
         Opamp(n1,n2,n5,n2);
         VCCS(" ",n5,n2,n1,n2,-1.0,false);
         return true;
    case 'F': // CCCS
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         n3=AddNodo(strtok(NULL," "));
         n4=AddNodo(strtok(NULL," "));
         n5=AddNodo("$"+String(extra++));
         ss=strtok(NULL," ");
         val = strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         Opamp(n5,n2,n3,n4);
         VCCS(" ",n3,n4,n5,n2,1.0,false);
         VCCS(nome,n1,n2,n5,n2,val,symb);
         return true;
    case 'Y':         // CCVS
    case 'K':
         nome=strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         n3=AddNodo(strtok(NULL," "));
         n4=AddNodo(strtok(NULL," "));
         ss=strtok(NULL," ");
         val = strtod(ss,NULL);
         ss=strtok(NULL," ");
         symb=(ss[0]=='0');
         Opamp(n1,n2,n3,n4);
         VCCS(nome,n3,n4,n1,n2,val,symb);
         return true;
    case 'O':
         strtok(riga.c_str()," ");
         n1=AddNodo(strtok(NULL," "));
         n2=AddNodo(strtok(NULL," "));
         n3=AddNodo(strtok(NULL," "));
         n4=AddNodo(strtok(NULL," "));
         Opamp(n1,n2,n3,n4);
         return true;
    default:
         return false;
  }
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Delta()
{ for(int i=0;i<OpAmp->Count;i++)
      { Componente *c=(Componente*)OpAmp->Items[i];
        if(!h1->Test(c->Nv[0],c->Nv[1]))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
        h1->Add(c->Nv[0],c->Nv[1]);
        cDet->SetV(c->Nv[0]-1,i,1);
        cDet->SetV(c->Nv[1]-1,i,-1);
        if(!h2->Test(c->Ni[0],c->Ni[1]))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
        h2->Add(c->Ni[0],c->Ni[1]);
        cDet->SetI(c->Ni[0]-1,i,1);
        cDet->SetI(c->Ni[1]-1,i,-1);
      }
   NNodi=Nodi->Count-OpAmp->Count;
   sDelta=0;
   D12=false;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Delta11()
{
   if(!h1->Test(NodoIng,NodoRef))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
   h1->Add(NodoIng,NodoRef);
   cDet->SetV(NodoIng-1,0,1);
   cDet->SetV(NodoRef-1,0,-1);
   if(!h2->Test(NodoIng,NodoRef))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
   h2->Add(NodoIng,NodoRef);
   cDet->SetI(NodoIng-1,0,1);
   cDet->SetI(NodoRef-1,0,-1);
   for(int i=0;i<OpAmp->Count;i++)
      { Componente *c=(Componente*)OpAmp->Items[i];
        if(!h1->Test(c->Nv[0],c->Nv[1]))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
        h1->Add(c->Nv[0],c->Nv[1]);
        cDet->SetV(c->Nv[0]-1,i+1,1);
        cDet->SetV(c->Nv[1]-1,i+1,-1);
        if(!h2->Test(c->Ni[0],c->Ni[1]))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
        h2->Add(c->Ni[0],c->Ni[1]);
        cDet->SetI(c->Ni[0]-1,i+1,1);
        cDet->SetI(c->Ni[1]-1,i+1,-1);
      }
   NNodi=Nodi->Count-1-OpAmp->Count;
   sDelta=1;
   D12=false;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Delta12()
{
    if(!h1->Test(NodOut1,NodOut2))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
    h1->Add(NodOut1,NodOut2);
    cDet->SetV(NodOut1-1,0,1);
    cDet->SetV(NodOut2-1,0,-1);
    if(!h2->Test(NodoIng,NodoRef))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
    h2->Add(NodoIng,NodoRef);
    cDet->SetI(NodoIng-1,0,1);
    cDet->SetI(NodoRef-1,0,-1);
    for(int i=0;i<OpAmp->Count;i++)
      { Componente *c=(Componente*)OpAmp->Items[i];
        if(!h1->Test(c->Nv[0],c->Nv[1]))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
        h1->Add(c->Nv[0],c->Nv[1]);
        cDet->SetV(c->Nv[0]-1,i+1,1);
        cDet->SetV(c->Nv[1]-1,i+1,-1);
        if(!h2->Test(c->Ni[0],c->Ni[1]))
          { ErrorMsg("Invalid Circuit!\r\nCheck the circuit schematic!");
            return;
          }
        h2->Add(c->Ni[0],c->Ni[1]);
        cDet->SetI(c->Ni[0]-1,i+1,1);
        cDet->SetI(c->Ni[1]-1,i+1,-1);
      }
    NNodi=Nodi->Count-1-OpAmp->Count;
    sDelta=1;
    D12=true;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Denom()
{
cDet=new CDet(Nodi->Count-1);
h1=new Graphs(Nodi->Count);
h2=new Graphs(Nodi->Count);
if(TipoIng=='V') Delta11();   // Denom = delta 11
else Delta();                 // Denom = delta
NBra=Compon->Count;
Find(false);
NTDen=NumT;
GradoDen=GradoMax;
TERM *Term=new TERM();
Term->Write(fp);
delete Term;
fclose(fp);
fp=fopen(FilePar.c_str(),"ab");
String riga=String(GradoNum)+" "+String(NTNum)+"\r\n";
fputs(riga.c_str(),fp);
riga=String(GradoDen)+" "+String(NTDen)+"\r\n";
fputs(riga.c_str(),fp);
fclose(fp);
fp=NULL;
delete h1;
delete h2;
delete cDet;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Numerat()
{
cDet=new CDet(Nodi->Count-1);
h1=new Graphs(Nodi->Count);
h2=new Graphs(Nodi->Count);
if(TipoIng=='I' && NodOut1==NodoIng) Delta11();   // Z in
else Delta12();
NBra=Compon->Count;
Find(true);
NTNum=NumT;
GradoNum=GradoMax;
TERM *Term=new TERM();
Term->Write(fp);
delete Term;
delete h1;
delete h2;
delete cDet;
}
//---------------------------------------------------------------------------
int __fastcall Circuito::Segno(MStack* stk)
{
int i,nc;
Componente* comp;
cDet->ClearP(OpAmp->Count+1);
for(i=0;i<stk->Count;i++)
   { nc=stk->Elem(i);
     comp=(Componente*)Compon->Items[nc];
     cDet->SetV(comp->Nv[0]-1,i+sDelta+OpAmp->Count,1);
     cDet->SetV(comp->Nv[1]-1,i+sDelta+OpAmp->Count,-1);
     cDet->SetI(comp->Ni[0]-1,i+sDelta+OpAmp->Count,1);
     cDet->SetI(comp->Ni[1]-1,i+sDelta+OpAmp->Count,-1);
   }
return cDet->Segno();
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Warn()
{
FWarn=true;
TMsgDlgButtons Buttons;
Buttons << mbYes << mbNo;
int res=MessageDlg("The output files for this circuit\r\ncould requires tens of MegaBytes!\r\nDo you want to proceed?",
                    mtWarning,Buttons,0);
if(res==mrNo) Abort=true;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::Find(bool Numerat)
{
if(Approx)
  { FindApprox(Numerat);
    return;
  }
TERM *Term=new TERM();
int i,nc,grado;
int NumAtt=0;
double coe;
int K=0;
NumT=0;
GradoMax=0;
Componente *Comp;
MStack* Stack=new MStack(NNodi-1);

lab2:
   if(Stack->Count==NNodi-1)
      { Term->Clear();
        if(NumAtt || OpAmp->Count || D12) coe=Segno(Stack);
        else coe=1.0;
        grado=0;
        for(i=0;i<Stack->Count;i++)
               { nc=Stack->Elem(i);
                 Comp=(Componente*)Compon->Items[nc];
                 if(!Comp->Imped)
                   { if(Comp->Symb) Term->Add(IDx[nc]);
                     else coe *= Comp->Val;
                     if(Comp->Reattivo) grado++;
                   }
               }
        for(i=0;i<Impedenze->Count;i++)
            { if(!Stack->Find(Impedenze->Strings[i].ToInt()))
                { nc=Impedenze->Strings[i].ToInt();
                  Comp=(Componente*)Compon->Items[nc];
                  if(Comp->Symb) Term->Add(IDx[nc]);
                  else coe *= Comp->Val;
                  if(Comp->Reattivo) grado++;
                }
            }
        Term->Coef=coe;
        Term->Pot=BYTE(grado);
        Term->Write(fp);
        NumT++;
        if(!FWarn && NumT>200000) Warn();
        if(grado>GradoMax) GradoMax=grado;
        Application->ProcessMessages();
        goto lab7;
      }
lab3:
   K++;
   if(Stack->Count + NBra - K < NNodi-2) goto lab6;
   Comp=(Componente*)Compon->Items[K-1];
   if(!h1->Test(Comp->Nv[0],Comp->Nv[1])) goto lab3;
   if(!h2->Test(Comp->Ni[0],Comp->Ni[1])) goto lab3;
   h1->Add(Comp->Nv[0],Comp->Nv[1]);
   h2->Add(Comp->Ni[0],Comp->Ni[1]);
   Stack->Insert(K-1);
   if(Comp->Attivo) NumAtt++;
   goto lab2;

lab6:
   if(Stack->Count==0 || Abort)
       { delete Stack;
         delete Term;
         return;
       }

lab7:
   K=Stack->Elem(0);
   Stack->Delete();
   Comp=(Componente*)Compon->Items[K];
   h1->Rem(Comp->Nv[0],Comp->Nv[1]);
   h2->Rem(Comp->Ni[0],Comp->Ni[1]);
   if(Comp->Attivo) NumAtt--;
   K++;
   Application->ProcessMessages();
   goto lab3;
}
//---------------------------------------------------------------------------
void __fastcall Circuito::FindApprox(bool Numerat)
{
TERM *Term=new TERM();
int i,nc,grado,j;
int NumAtt=0;
double coe,erro;
bool stop;
complex<double> totamp[10], amp;
for(j=0;j<NumPuls;j++) totamp[j]=complex<double>(0.0,0.0);
int K=0;
NumT=0;
GradoMax=0;
Componente *Comp;
MStack* Stack=new MStack(NNodi-1);

alab2:
   if(Stack->Count==NNodi-1)
      { Term->Clear();
        if(NumAtt || OpAmp->Count || D12) coe=Segno(Stack);
        else coe=1.0;
        amp=1.0;
        grado=0;
        for(i=0;i<Stack->Count;i++)
               { nc=Stack->Elem(i);
                 Comp=(Componente*)Compon->Items[nc];
                 if(!Comp->Imped)
                   { if(Comp->Symb) Term->Add(IDx[nc]);
                     else coe *= Comp->Val;
                     if(Comp->Reattivo) grado++;
                     amp *= Comp->Val;
                   }
               }
        for(i=0;i<Impedenze->Count;i++)
            { if(!Stack->Find(Impedenze->Strings[i].ToInt()))
                { nc=Impedenze->Strings[i].ToInt();
                  Comp=(Componente*)Compon->Items[nc];
                  if(Comp->Symb) Term->Add(IDx[nc]);
                  else coe *= Comp->Val;
                  if(Comp->Reattivo) grado++;
                  amp *= Comp->Val;
                }
            }
        for(j=0;j<NumPuls;j++) totamp[j] += amp*pow(complex<double>(0.0,Pulsaz[j]),grado);
        stop=true;
        for(j=0;j<NumPuls;j++)
            { if(Numerat) erro=fabs(abs(ValNum[j])-abs(totamp[j]))/abs(ValNum[j]);
              else erro=fabs(abs(ValDen[j])-abs(totamp[j]))/abs(ValDen[j]);
              if(erro>Limite)
                 { stop=false;
                   break;
                 }
            }
        Term->Coef=coe;
        Term->Pot=BYTE(grado);
        Term->Write(fp);
        NumT++;
        if(!FWarn && NumT>200000) Warn();
        if(grado>GradoMax) GradoMax=grado;
        Application->ProcessMessages();
        if(stop)
           { delete Stack;
             delete Term;
             return;
           }
        goto alab7;
      }
alab3:
   K++;
   if(Stack->Count + NBra - K < NNodi-2) goto alab6;
   Comp=(Componente*)Compon->Items[K-1];
   if(!h1->Test(Comp->Nv[0],Comp->Nv[1])) goto alab3;
   if(!h2->Test(Comp->Ni[0],Comp->Ni[1])) goto alab3;
   h1->Add(Comp->Nv[0],Comp->Nv[1]);
   h2->Add(Comp->Ni[0],Comp->Ni[1]);
   Stack->Insert(K-1);
   if(Comp->Attivo) NumAtt++;
   goto alab2;

alab6:
   if(Stack->Count==0 || Abort)
       { delete Stack;
         delete Term;
         return;
       }

alab7:
   K=Stack->Elem(0);
   Stack->Delete();
   Comp=(Componente*)Compon->Items[K];
   h1->Rem(Comp->Nv[0],Comp->Nv[1]);
   h2->Rem(Comp->Ni[0],Comp->Ni[1]);
   if(Comp->Attivo) NumAtt--;
   K++;
   Application->ProcessMessages();
   goto alab3;
}



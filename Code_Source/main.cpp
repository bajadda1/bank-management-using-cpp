#include <iostream>
#include <string>
#include <windows.h>
#include<vector>
#include <fstream>
#include <conio.h> // pour getch()

using namespace std;
int choix;
string code;
/*calculate sold with interest rate */
double solde_xannee(double S,float TI,int annee)
{
    double solde_var=S;
    for (int i=1; i<=annee; i++)
    {
        solde_var=solde_var+TI/100*solde_var;
    }
    return solde_var;
}
/*calculate how many years needed to double solde */
int doublesolde_annee(double S,float TI)
{
    int annee=0;
    double solde_double=2*S;
    double solde_var=S;
    while (solde_var<solde_double)
    {
        solde_var=solde_var+solde_var*TI/100;
        annee++;
    }
    return annee;
}

//handle exeption
class RetraitInterditException
{
public:
    RetraitInterditException() {}
    string affiche_err()
    {
        return "\t\t\t\t\t\Retrait impossible : solde insuffisant.";
    }
};

//define class person
class PersonnePhysique
{
private:
    //attributes
    string nom_m;
    string prenom_m;

public:
    //constructor
    PersonnePhysique(string nom, string prenom)
    {
        nom_m = nom;
        prenom_m = prenom;
    }
    //getter and setter
    string getNom()
    {
        return nom_m;
    }
    void setNom(string nom)
    {
        nom_m=nom;
    }

    string getPrenom()
    {
        return prenom_m;
    }
     void setPreom(string prenom)
    {
        prenom_m=prenom;
    }
};
//define banc account class
class CompteBancaire:public PersonnePhysique //inherit from person class
{
private:
    double solde_m;
    string code_m;
    int decouvertAutorise_m;
    static int nombreComptes_m;//static attributes to be incremented automatically
    static int nombreComptesDebiteurs_m;

public:
    static vector<CompteBancaire*> List_CompteBancaires; //save banc accounts
    CompteBancaire(string nom="inconnu",string prenom="inconnu"):PersonnePhysique(nom,prenom)
    {
        solde_m = 0;
        code_m = "NULL";
        decouvertAutorise_m = 0;
        nombreComptes_m++;//increment each time a new account addded
        List_CompteBancaires.push_back(this);// Stocker l'objet créé dans le vecteur 'List_CompteBancaires'
    }

    CompteBancaire(string nom,string prenom,string code,double soldeInitial,int decouvertAutorise=0):PersonnePhysique(nom,prenom)
    {
        solde_m = soldeInitial;
        code_m = code;
        decouvertAutorise_m=decouvertAutorise;
        nombreComptes_m++;
        List_CompteBancaires.push_back(this);// Stocker l'objet créé dans le vecteur 'List_CompteBancaires'


        if (soldeInitial < 0)
        {
            nombreComptesDebiteurs_m++;
        }
    }

    void setCode(string code)
    {
        code_m = code;
    }
    string getCode()
    {
        return code_m;
    }

    double getSolde()
    {
        return solde_m;
    }

    void depot(double montant, string code)
    {
        if (code_m != code)
        {
            cout << "Code incorrect" << endl;
            return;
        }

        solde_m += montant;

        if (solde_m >= 0)
        {
            nombreComptesDebiteurs_m--;
        }
    }

    void retrait(double montant, string code)
    {
        if (code_m != code)
        {
            cout << "Code incorrect\n" << endl;
            return;
        }

        if (montant > solde_m + decouvertAutorise_m)
        {
            throw RetraitInterditException();
            return;
        }

        solde_m -= montant;

        if (solde_m < 0 )
        {
            nombreComptesDebiteurs_m++;
        }
    }

    void SetDecouvertAutorise(double montant)
    {
        decouvertAutorise_m = montant;
    }
    int getDecouvertAutorise()
    {
        return decouvertAutorise_m;
    }

    static int getNombreComptes()
    {
        return nombreComptes_m;
    }

    static int getNombreComptesDebiteurs()
    {
        return nombreComptesDebiteurs_m;
    }

};

vector<CompteBancaire*> CompteBancaire::List_CompteBancaires;
int CompteBancaire::nombreComptes_m = 0;
int CompteBancaire::nombreComptesDebiteurs_m = 0;
void menu();
void entrer_code();
void color(int text_color, int bg_color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = text_color + (bg_color * 16);
    SetConsoleTextAttribute(hConsole, color);
}
void information(CompteBancaire* X );



int main()
{
    string nom;
    string prenom;
    double soldeInitial;

    int c=0;
    int montant;


    CompteBancaire compte1("JACK","HALMES","0000",900);
    CompteBancaire compte2("AHMED","BAJADDA","0688bd",1000);
    CompteBancaire compte3("YASSER","JERAF","00100",10000);
    CompteBancaire compte4("OUSSAMA","MAZOUZ","0099100",1100);
Refaire1:
    {
        menu();
        switch(choix)
        {
        case 1:
        {
            system("cls");
            cout<<"\t\t|=>votre nom svp:";
            cin>>nom;
            cout<<"\t\t|=>votre prenom svp:";
            cin>>prenom;
Refaire2:
            {
                cout<<"\t\t|=>votre code svp :";
                cin>>code;

                for (int i=0; i<CompteBancaire::List_CompteBancaires.size(); i++)
            {
                if (code==(CompteBancaire::List_CompteBancaires[i]->getCode()))
                {
                    color(4,0);
                    cout<<"\t\t|=>ce code deja exist! try another svp!\n";
                    color(11,0);
                    goto Refaire2;
                }
            }


            }


            cout<<"\t\t|=>solde initiale svp :";
            cin>>soldeInitial;
            CompteBancaire objet(nom,prenom,code,soldeInitial);
            color(11,0);
            cout<<"\n\t\t|-->return to menu?[1 si oui Anything else exit ]:"<<endl;
            cin>>choix;
            if(choix==1)
            {
                system("cls");
                goto Refaire1;
            }
            else
                break;

        }
        case 2:
        {
            system("cls");
            entrer_code();
            for (int i=0; i<CompteBancaire::List_CompteBancaires.size(); i++)
            {
                if (code==(CompteBancaire::List_CompteBancaires[i]->getCode()))
                {
                    system("cls");
                    information((CompteBancaire::List_CompteBancaires[i]));
                    break;
                }
                c++;
            }
            if(c==CompteBancaire::List_CompteBancaires.size())
            {
                color(7,0);
                c=0;
                color(4,0);
                cout<<"\n\t\t|-->aucun compte a ce code"<<endl;
                system("pause");
                system("cls");
                goto Refaire1;
            }
            color(11,0);
            cout<<"\n\t\t|-->return to menu?[1 si oui Anything else exit ]:"<<endl;
            cin>>choix;
            if(choix==1)
            {
                system("cls");
                goto Refaire1;
            }
            else
                break;


        }
        case 3:
        {
            system("cls");
            entrer_code();
            for (int i=0; i<CompteBancaire::List_CompteBancaires.size(); i++)
            {
                if (code==(CompteBancaire::List_CompteBancaires[i]->getCode()))
                {
                    system("cls");
                    cout<<"\n\t\t\t\t\t\t=>votre solde est :"<<CompteBancaire::List_CompteBancaires[i]->getSolde()<<endl;
                    cout<<"\n\t\t\t\t\t\=>combien voulez vous deposer:";
                    cin>>montant;
                    CompteBancaire::List_CompteBancaires[i]->depot(montant,code);

                    break;

                }
                c++;
            }
            if(c==CompteBancaire::List_CompteBancaires.size())
            {
                c=0;
                color(4,0);
                cout<<"\n\t\t|-->aucun compte a ce code"<<endl;
                system("pause");
                system("cls");
                goto Refaire1;
            }
            cout<<"\n\t\t|-->return to menu?[1 si oui Anything else exit ]:"<<endl;
            cin>>choix;
            if(choix==1)
            {
                system("cls");
                goto Refaire1;
            }
            else

                break;
        }
        case 4:
        {
            system("cls");
            entrer_code();
            for (int i=0; i<CompteBancaire::List_CompteBancaires.size(); i++)
            {
                if (code==(CompteBancaire::List_CompteBancaires[i]->getCode()))
                {
                    system("cls");
                    cout<<"\n\t\t\t\t\t\t|=>votre solde est :"<<CompteBancaire::List_CompteBancaires[i]->getSolde()<<endl;
                    cout<<"\n\t\t\t\t\t\|=>combien voulez vous retraire:";
                    cin>>montant;
                    try
                    {
                        CompteBancaire::List_CompteBancaires[i]->retrait(montant,code);
                    }
                    catch(RetraitInterditException er)
                    {
                        color(4,0);
                        cout<<er.affiche_err();
                        color(2,0);

                    }

                    break;
                }
                c++;
            }
            if(c==CompteBancaire::List_CompteBancaires.size())
            {
                c=0;
                color(4,0);
                cout<<"\n\t\t|-->aucun compte a ce code"<<endl;
                system("pause");
                system("cls");
                goto Refaire1;
            }
            color(11,0);
            cout<<"\n\t\t|-->return to menu?[1 si oui Anything else exit ]:"<<endl;
            cin>>choix;
            if(choix==1)
            {
                system("cls");
                goto Refaire1;
            }
            else

                break;
        }
        case 5:
        {
            system("cls");
            entrer_code();
            for (int i=0; i<CompteBancaire::List_CompteBancaires.size(); i++)
            {
                if (code==(CompteBancaire::List_CompteBancaires[i]->getCode()))
                {
                    int annee=0;
                    float TI=0;
                    system("cls");
                    cout<<"\t\t\t\t\t\t|=>votre solde est :"<<CompteBancaire::List_CompteBancaires[i]->getSolde()<<endl;

                    cout<<"\t\t\t\t\t\t|=>entrez le nomber d'annees:";
                    cin>>annee;
                    cout<<"\t\t\t\t\t\t|=>Taut d'interet TI:";
                    cin>>TI;
                    cout<<"\t\t|=>votre solde dans "<<annee<<" annees avec un taux d interet de "<<TI<<" % fera:";
                    cout<<solde_xannee(CompteBancaire::List_CompteBancaires[i]->getSolde(),TI,annee)<<endl;
                    cout<<"\t\t|=>votre solde sera double dans "<< doublesolde_annee(CompteBancaire::List_CompteBancaires[i]->getSolde(),TI)<< " annees";
                    cout<<endl;


                    break;

                }
                c++;
            }
            if(c==CompteBancaire::List_CompteBancaires.size())
            {
                c=0;
                color(4,0);
                cout<<"\n\t\t|-->aucun compte a ce code"<<endl;
                system("pause");
                system("cls");
                goto Refaire1;
            }
            color(11,0);
            cout<<"\n\t\t|-->return to menu?[1 si oui Anything else exit ]:"<<endl;
            cin>>choix;
            if(choix==1)
            {
                system("cls");
                goto Refaire1;
            }
            else

                break;

        }
        case 6:
        {
            color(4,0);
            cout<<"                           voulez vous vraiment sortir?????"<<endl;
            color(11,0);
            cout<<"                           [1 si oui anything else return to menu]:";
            cin>>choix;
            if(choix==1)
                break;
            else
            {
                system("cls");
                goto Refaire1;


            }
            break;

        }
        default:
        {
            color(4,0);
            printf("          svp votre choix est hors liste!!\n");
            system("pause");
            system("cls");
            goto Refaire1;
            break;
        }
        }
    }
// ouverture du fichier en mode écriture
    std::ofstream fichier("ah.txt");

    // vérification que le fichier a été ouvert correctement
    if (!fichier.is_open())
    {
        cout << "Erreur : impossible d'ouvrir le fichier" << std::endl;
    }

    // sauvgarder les objets dans le fichier
    fichier <<"NOM        |====| "<<"PRENOM      |====| "<<"SOLDE          \n";
    for (int i=0; i<CompteBancaire::List_CompteBancaires.size(); i++)
    {
        fichier <<CompteBancaire::List_CompteBancaires[i]->getNom()<<"|====|";
        fichier <<CompteBancaire::List_CompteBancaires[i]->getPrenom()<<"|====|";
        fichier <<CompteBancaire::List_CompteBancaires[i]->getSolde()<<"\n\n";

    }
    // fermeture du fichier
    fichier.close();



    return 0;
}

void menu()
{
    color(14,0);
    printf("                          ________________________\n");
    printf("\n                           BIENVENUE AU MENU (\):");
    printf("\n                          ________________________\n\n");
    printf("\t                           _______________________\n");
    printf("\t                          |                       | \n");
    printf("\t                          |    1 >   INSCRIPTION  |\n");
    printf("\t                          |_______________________|\n");
    printf("\t                           _______________________\n");
    printf("\t                          |                       | \n");
    printf("\t                          |    2 >   COMPTE       |\n");
    printf("\t                          |_______________________|\n");
    printf("\t                           _______________________\n");
    printf("\t                          |                       | \n");
    printf("\t                          |    3 >   DEPOT        |\n");
    printf("\t                          |_______________________|\n");
    printf("\t                           _______________________\n");
    printf("\t                          |                       | \n");
    printf("\t                          |    4 >   RETRAIT      |\n");
    printf("\t                          |_______________________|\n");
    printf("\t                           _______________________\n");
    printf("\t                          |                       | \n");
    printf("\t                          |    5 >   INTERET      |\n");
    printf("\t                          |_______________________|\n");
    printf("\t                           _______________________\n");
    printf("\t                          |                       | \n");
    printf("\t                          |    6 >   E X I T      |\n");
    printf("\t                          |_______________________|\n");
    color(11,0);
    printf("\n\n                           |-->Entrer votre choix:");
    cin>>choix;
}
void entrer_code()
{
    string password;
    printf("\t                           _______________________________\n");
    printf("\t                          |                               | \n");
    printf("\t                          |    SVP ENTRER VOTRE CODE !!   |\n");
    printf("\t                          |_______________________________|\n");
    printf("\n\n                                   |-->Entrer votre code ici:");
    char c;
    while ((c = getch()) != '\r')   // '\r' correspond à la touche "Entrée"
    {
        password+=c;
        cout << "*";
    }
    code=password;
    //cout << code;

}
void information(CompteBancaire *X )
{
    cout<<" \n\n\n\t\t\t\t\t\tNOM:"<<X->getNom()<<endl;
    cout<<" \t\t\t\t\t\tPRENOM:"<<X->getPrenom()<<endl;
    cout<<" \t\t\t\t\t\tSOLDE:"<<X->getSolde()<<endl;
    cout<<" \t\t\t\t\t\tDECOUVERT AUTORISE :"<<X->getDecouvertAutorise()<<endl;

}

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// --- Classe Matiere ---
class Matiere {
private:
    string code;
    string libelle;
    float coefficient;

public:
    Matiere(string c = "", string l = "", float coef = 1.0f) 
        : code(c), libelle(l), coefficient(coef) {}

    string getCode() const { return code; }
    string getLibelle() const { return libelle; }
    float getCoefficient() const { return coefficient; }
};

// --- Classe UE (Unité d'Enseignement) ---
class UE {
private:
    string code;
    string libelle;
    int credit;
    vector<Matiere> matieres;

public:
    UE(string c = "", string l = "", int cred = 0) 
        : code(c), libelle(l), credit(cred) {}

    void ajouterMatiere(const Matiere& m) {
        matieres.push_back(m);
    }

    string getCode() const { return code; }
    string getLibelle() const { return libelle; }
    int getCredit() const { return credit; }
    const vector<Matiere>& getMatieres() const { return matieres; }
};

// --- Classe Filiere ---
class Filiere {
private:
    string code;
    string nom;
    vector<UE> ues;

public:
    Filiere(string c = "", string n = "") : code(c), nom(n) {}

    void ajouterUE(const UE& ue) {
        ues.push_back(ue);
    }

    string getCode() const { return code; }
    string getNom() const { return nom; }
    const vector<UE>& getUes() const { return ues; }
};

// --- Classe Note ---
class Note {
private:
    string codeMatiere;
    float valeur; // Sur 20

public:
    Note(string codeMat = "", float val = 0.0f) 
        : codeMatiere(codeMat), valeur(val) {}

    string getCodeMatiere() const { return codeMatiere; }
    float getValeur() const { return valeur; }
    void setValeur(float val) { valeur = val; }
};

// --- Classe Etudiant ---
class Etudiant {
private:
    string numeroEtudiant;
    string nom;
    string prenom;
    string niveau; // L1, L2, L3
    const Filiere* filiere;
    vector<Note> notes;

public:
    Etudiant(string num, string n, string p, string niv, const Filiere* fil)
        : numeroEtudiant(num), nom(n), prenom(p), niveau(niv), filiere(fil) {}

    void ajouterNote(const string& codeMat, float valeur) {
        for (size_t i = 0; i < notes.size(); i++) {
            if (notes[i].getCodeMatiere() == codeMat) {
                notes[i].setValeur(valeur);
                return;
            }
        }
        notes.push_back(Note(codeMat, valeur));
    }

    float getNoteMatiere(const string& codeMat) const {
        for (size_t i = 0; i < notes.size(); i++) {
            if (notes[i].getCodeMatiere() == codeMat) {
                return notes[i].getValeur();
            }
        }
        return -1.0f; // Non noté
    }

    // Déterminer si un étudiant valide une UE (Moyenne pondérée des matières de l'UE >= 10/20)
    bool validerUE(const UE& ue) const {
        float sommePondereeUE = 0.0f;
        float sommeCoeffsUE = 0.0f;
        int matieresNotees = 0;

        const vector<Matiere>& mats = ue.getMatieres();
        for (size_t i = 0; i < mats.size(); i++) {
            float noteVal = getNoteMatiere(mats[i].getCode());
            if (noteVal >= 0.0f) {
                sommePondereeUE += noteVal * mats[i].getCoefficient();
                sommeCoeffsUE += mats[i].getCoefficient();
                matieresNotees++;
            }
        }

        if (matieresNotees == 0 || sommeCoeffsUE == 0.0f) return false;
        return (sommePondereeUE / sommeCoeffsUE) >= 10.0f;
    }

    // Calculer la moyenne générale pondérée de l'étudiant
    float calculerMoyenneGenerale() const {
        if (filiere == NULL) return 0.0f;

        float sommePondereeTotale = 0.0f;
        float sommeCoeffsTotale = 0.0f;

        const vector<UE>& ues = filiere->getUes();
        for (size_t i = 0; i < ues.size(); i++) {
            const vector<Matiere>& mats = ues[i].getMatieres();
            for (size_t j = 0; j < mats.size(); j++) {
                float noteVal = getNoteMatiere(mats[j].getCode());
                if (noteVal >= 0.0f) {
                    sommePondereeTotale += noteVal * mats[j].getCoefficient();
                    sommeCoeffsTotale += mats[j].getCoefficient();
                }
            }
        }

        if (sommeCoeffsTotale == 0.0f) return 0.0f;
        return sommePondereeTotale / sommeCoeffsTotale;
    }

    // Afficher le relevé de notes complet
    void afficherReleve() const {
        cout << "========================================\n";
        cout << "         RELEVE DE NOTES                \n";
        cout << "========================================\n";
        cout << "Étudiant : " << prenom << " " << nom << " (ID: " << numeroEtudiant << ")\n";
        cout << "Niveau   : " << niveau << "\n";
        if (filiere != NULL) {
            cout << "Filière  : " << filiere->getNom() << " (" << filiere->getCode() << ")\n";
        }
        cout << "----------------------------------------\n";

        if (filiere != NULL) {
            const vector<UE>& ues = filiere->getUes();
            for (size_t i = 0; i < ues.size(); i++) {
                cout << "UE : " << ues[i].getCode() << " - " << ues[i].getLibelle() 
                     << " (Crédits : " << ues[i].getCredit() << ")\n";
                
                float sommePondUE = 0.0f;
                float sumCoeffUE = 0.0f;
                int count = 0;

                const vector<Matiere>& mats = ues[i].getMatieres();
                for (size_t j = 0; j < mats.size(); j++) {
                    float note = getNoteMatiere(mats[j].getCode());
                    cout << "  * " << mats[j].getCode() << " - " << mats[j].getLibelle() 
                         << " (Coef " << mats[j].getCoefficient() << ") : ";
                    if (note >= 0.0f) {
                        cout << note << "/20\n";
                        sommePondUE += note * mats[j].getCoefficient();
                        sumCoeffUE += mats[j].getCoefficient();
                        count++;
                    } else {
                        cout << "Non noté\n";
                    }
                }

                if (count > 0 && sumCoeffUE > 0.0f) {
                    float moyUE = sommePondUE / sumCoeffUE;
                    cout << "  --> Moyenne UE : " << fixed << setprecision(2) << moyUE << "/20 | "
                         << (validerUE(ues[i]) ? "[ VALIDÉE ]" : "[ NON VALIDÉE ]") << "\n";
                } else {
                    cout << "  --> UE non évaluée\n";
                }
                cout << "----------------------------------------\n";
            }
        }

        cout << "MOYENNE GÉNÉRALE : " << fixed << setprecision(2) << calculerMoyenneGenerale() << "/20\n";
        cout << "========================================\n";
    }
};

// --- Fonction Principale (Test) ---
int main() {
    // Création d'une filière
    Filiere info("INFO", "Informatique");

    // Création des UE et Matières
    UE ue1("UE1", "Fondements Mathématiques", 6);
    ue1.ajouterMatiere(Matiere("MAT101", "Algèbre", 2.0f));
    ue1.ajouterMatiere(Matiere("MAT102", "Analyse", 3.0f));

    UE ue2("UE2", "Programmation", 8);
    ue2.ajouterMatiere(Matiere("INF101", "Algorithmique", 4.0f));

    info.ajouterUE(ue1);
    info.ajouterUE(ue2);

    // Inscription d'un étudiant
    Etudiant etu1("ETU2026", "Fadol", "Adio", "L2", &info);

    // Ajout des notes
    etu1.ajouterNote("MAT101", 12.5f);
    etu1.ajouterNote("MAT102", 14.0f);
    etu1.ajouterNote("INF101", 15.0f);

    // Affichage du relevé de notes
    etu1.afficherReleve();

    return 0;
}

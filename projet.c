/********************************************************************************
 * @name   Gestion d'absences pour les étudiants en IUT.                        *
 * @author Raphaël Rispaud (Groupe 112 / @Raph9213 / Binôme 73)                 *
 * @date   2024-10-23                                                           *
 * @brief  Ce programme permet de gérer les absences des étudiants,             *
 *         d'enregistrer des absences, de valider les justificatifs,            *
 *         et de suivre les étudiants en fonction de leurs absences.            *
 *         Les absences peuvent être justifiées ou non.                         *
 *                                                                              *
 * @file : projet.c                                                             *
 * @version : 1.0                                                               *
 * @license: MIT                                                                *
 * @license-URL: https://opensource.org/licenses/MIT                            *
 * @compatibility: Windows, Linux, Mac                                          *
 * @usage: gcc projet.c -o gestion_absences                                     *
 * @git: https://local.raph9213.xyz/IUTProjet/projet.git                        *
 * @branch: main                                                                *
 * @changelog: Release initiale                                                 *
 * @warning: force-force-push, force-pull-request, default-fork, default-clone  *
 *                                                                              *
 * @note                                                                        *
 *   Do not remove this header. This code is meant to be compiled in Visual     *
 *   Studio; if you don't, you may remove the '#pragma warning(disable : 4996)' *
 *   line.                                                                      *
 *                                                                              *
 *   This code is compatible with the Doxygen format, which allows you to       *
 *   generate documentation for your code.                                      *
 ********************************************************************************/

/********************************************************************************
 *                              [Constantes Globales]                           *
 *                                                                              *
 *  Cette section regroupe toutes les constantes globales utilisées dans le     *
 *  programme. Les constantes définissent des valeurs maximales pour le nombre  *
 *  d'étudiants, d'absences, ainsi que d'autres limites importantes.            *
 ********************************************************************************/

/* Le code ici est modifiable, si un changement a lieu. */

/**
 * @defgroup Constantes Constantes Globales
 * @brief Définition des constantes globales utilisées dans le programme.
 * @{
 */
enum {
    MAX_ETUDIANTS = 100,                /** Nombre maximal d'étudiants autorisés */
    MAX_ABSENCES = 200,                 /** Nombre maximal d'absences accepté par le programme */
    MAX_NOM = 31,                       /** Taille maximale du nom d'un étudiant */
    MAX_JUSTIFICATIF = 51,              /** Taille maximale du justificatif */
    NB_JOURS_SEMESTRE = 40,             /** Nombre de jours dans un semestre */
    LIMITE_ABSENCES_NON_JUSTIFIEES = 5, /** Limite d'absences non justifiées */
    MAX_COMMANDE = 100,                 /** Taille maximale d'une commande */
    DELAI_JUSTIFICATIF = 3,             /** Délai pour soumettre un justificatif */
    JOUR_DEBUT_SEMESTRE = 1             /** Jour de début du semestre */
};
/** @} */


/* ! MERCI DE NE PAS TOUCHER AU CODE A PARTIR D'ICI ! */

/********************************************************************************
 *                                [Bibliothèques]                               *
 *                                                                              *
 *  Cette section regroupe les bibliothèques standard utilisées dans le         *
 *  programme. Elles incluent les fonctions nécessaires pour les opérations     *
 *  d'entrée/sortie, de manipulation de chaînes de caractères, et de gestion    *
 *  de la mémoire.                                                              *
 *  Explication :                                                               *
 * - stdio.h : Entrée/sortie standard (printf, scanf, etc.)                     *
 * - string.h : Manipulation de chaînes de caractères (strcpy, strcmp, etc.)    *
 * - stdlib.h : Fonctions de conversion de chaînes en valeurs (atoi, etc.)      *
 * - pragma : Désactive un avertissement spécifique du compilateur MSVC         *
 ********************************************************************************/

#include <stdio.h> // printf, fgets
#include <string.h> // strtok, strcpy, strcmp, strlen
#include <stdlib.h> // atoi
#ifdef _MSC_VER // Verifie si le compilateur est MSVC
#pragma warning(disable : 4996, 6262)
#endif


/********************************************************************************
 *                            [Structures de Données]                           *
 *                                                                              *
 *  Cette section regroupe toutes les structures de données utilisées pour      *
 *  représenter les étudiants et leurs absences. Chaque structure inclut les    *
 *  informations nécessaires pour gérer les absences des étudiants et suivre    *
 *  leur état au cours du semestre.                                             *
 ********************************************************************************/

/**
 * @enum CodeCommande
 * @brief Représente les commandes possibles pour le programme.
 * @param CMD_INSCRIPTION Inscription d'un nouvel étudiant.
 * @param CMD_ABSENCE Enregistrement d'une absence.
 * @param CMD_JUSTIFICATIF Enregistrement d'un justificatif.
 * @param CMD_VALIDATION Validation d'un justificatif.
 * @param CMD_ETUDIANT Affichage des informations d'un étudiant.
 * @param CMD_ETUDIANTS Affichage de la liste des étudiants.
 * @param CMD_VALIDATIONS Affichage des absences en attente de validation.
 * @param CMD_DEFAILLANTS Affichage des étudiants défaillants.
 * @param CMD_EXIT Sortie du programme.
 * @param CMD_INCONNUE Commande inconnue.
 */
typedef enum CodeCommande {
    CMD_INSCRIPTION,
    CMD_ABSENCE,
    CMD_JUSTIFICATIF,
    CMD_VALIDATION,
    CMD_ETUDIANT,
    CMD_ETUDIANTS,
    CMD_VALIDATIONS,
    CMD_DEFAILLANTS,
    CMD_EXIT,
    CMD_INCONNUE
} CodeCommande;


/**
 * @defgroup Types Types de Données
 * @brief Définitions des types utilisés dans le programme.
 * @{
 */

/**
 * @enum DemiJournee
 * @brief Représente une demi-journée (matin ou après-midi).
 */
typedef enum DemiJournee { 
    MATIN,         /**< Représente le matin */
    APRES_MIDI     /**< Représente l'après-midi */
} DemiJournee;

/**
 * @enum StatutAbsence
 * @brief Statut d'une absence (justifiée, en attente, etc.).
 */
typedef enum StatutAbsence {
    EN_ATTENTE_JUSTIFICATIF, /**< Absence en attente de justificatif */
    EN_ATTENTE_VALIDATION,   /**< Justificatif en attente de validation */
    JUSTIFIEE,               /**< Absence justifiée */
    NON_JUSTIFIEE            /**< Absence non justifiée */
} StatutAbsence;

/**
 * @struct Etudiant
 * @brief Structure représentant un étudiant.
 * 
 * Cette structure contient les informations essentielles sur un étudiant,
 * y compris son identifiant, son nom, son groupe et les statistiques d'absences.
 */
typedef struct Etudiant {
    int id;                            /**< Identifiant unique de l'étudiant */
    char nom[MAX_NOM];                 /**< Nom de l'étudiant */
    int groupe;                        /**< Groupe de l'étudiant */
    int nbAbsencesNonJustifiees;       /**< Nombre d'absences non justifiées */
    int nbTotalAbsences;               /**< Nombre total d'absences */
} Etudiant;

/**
 * @struct Absence
 * @brief Structure représentant une absence d'un étudiant.
 * 
 * Cette structure contient toutes les informations liées à une absence,
 * telles que la date, la demi-journée, le statut, et le justificatif.
 */
typedef struct Absence {
    int id;                              /**< Identifiant unique de l'absence */
    int etudiantId;                      /**< Identifiant de l'étudiant concerné */
    int jour;                            /**< Jour de l'absence */
    DemiJournee demiJournee;             /**< Demi-journée de l'absence */
    StatutAbsence statut;                /**< Statut de l'absence */
    char justificatif[MAX_JUSTIFICATIF]; /**< Justificatif lié à l'absence */
    int jourJustificatif;                /**< Jour où le justificatif a été soumis */
} Absence;

/** @} */

/********************************************************************************
 *                           [Prototypes des Fonctions]                         *
 *                                                                              *
 *  Cette section contient les prototypes de toutes les fonctions utilisées     *
 *  dans le programme. Cela facilite la lecture du code et la correction.       *
 ********************************************************************************/

/* Fonctions Utilitaires */
void lireLigne(char *buffer, int taille);
int lireEntier(const char *token);
DemiJournee lireDemiJournee(const char *token);
int calculerJourCourant(Absence *absences, int nbAbsences);

/* Comparateurs pour qsort */
int comparerAbsencesParDate(const void *a, const void *b);
int comparerAbsences(const void *a, const void *b);
int comparerEtudiants(const void *a, const void *b);

/* Gestion des Étudiants */
void inscrireEtudiant(Etudiant *etudiants, int *nbEtudiants, const char *nom, int groupe);
Etudiant* trouverEtudiantParId(const Etudiant *etudiants, int nbEtudiants, int etudiantId);
void calculerAbsencesEtudiants(Etudiant *etudiants, int nbEtudiants, const Absence *absences, int nbAbsences, int jourCourant);

/* Gestion des Absences */
void enregistrerAbsence(Absence *absences, int *nbAbsences, const Etudiant *etudiants, int nbEtudiants, int etudiantId, int jour, DemiJournee demiJournee);
Absence* trouverAbsenceParId(const Absence *absences, int nbAbsences, int absenceId);
StatutAbsence determinerStatutAbsence(const Absence *absence, int jourCourant);

/* Gestion des Justificatifs */
void enregistrerJustificatif(Absence *absences, int nbAbsences, int absenceId, int jour, const char *justificatif);
void validerAbsence(Absence *absences, int nbAbsences, int absenceId, const char *code, int jourCourant);

/* Affichage */
void afficherErreur(const char *message);
void afficherEtudiant(Etudiant *etudiants, int nbEtudiants, Absence *absences, int nbAbsences, int etudiantId, int jourCourant);
void afficherEtudiants(Etudiant *etudiants, int nbEtudiants, const Absence *absences, int nbAbsences, int jourCourant);
void afficherAbsencesEnAttente(Absence *absences, int nbAbsences, Etudiant *etudiants, int nbEtudiants, int jourCourant);
void afficherAbsence(const Absence *absence, int afficherJustificatif);
void afficherDefaillants(Etudiant *etudiants, int nbEtudiants, const Absence *absences, int nbAbsences, int jourCourant);

/* Commandes Utilisateur */
void quitterProgramme();
CodeCommande obtenirCodeCommande(const char *cmd);
void traiterCommande(char *commande, Etudiant *etudiants, Absence *absences, int *nbEtudiants, int *nbAbsences);

/* Point d'Entrée */
int main();


/********************************************************************************
 *                            [Fonctions Utilitaires]                           *
 *                                                                              *
 *  Cette section regroupe les fonctions utilitaires utilisées dans l'ensemble  *
 *  du programme, telles que la lecture d'une ligne de texte depuis l'entrée    *
 *  standard ou la conversion de chaînes de caractères en valeurs spécifiques.  *
 ********************************************************************************/


/**
 * @ingroup Fonctions_utilitaires
 * @brief Lit une ligne de l'entrée standard.
 * 
 * Cette fonction lit une ligne de texte depuis l'entrée standard et supprime
 * le caractère de nouvelle ligne s'il est présent.
 * 
 * @param buffer Pointeur vers un buffer où stocker la ligne lue.
 * @param taille Taille maximale du buffer.
 */
void lireLigne(char *buffer, int taille) {
    fgets(buffer, taille, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

/**
 * @ingroup Fonctions_utilitaires
 * @brief Lit un entier à partir d'une chaîne de caractères.
 * 
 * Cette fonction convertit une chaîne de caractères en un entier.
 * 
 * @param token Chaîne de caractères représentant un entier.
 * @return Valeur entière convertie, ou -1 en cas d'erreur.
 */
int lireEntier(const char *token) {
    if (token == NULL) {
        return -1;
    }
    return atoi(token);
}

/**
 * @ingroup Fonctions_utilitaires
 * @brief Lit une demi-journée à partir d'une chaîne de caractères.
 * 
 * Cette fonction convertit une chaîne de caractères en une demi-journée (matin ou après-midi).
 * Utilise "am" pour le matin et "pm" pour l'après-midi.
 * 
 * @param token Chaîne de caractères représentant la demi-journée.
 * @return DemiJournee Valeur convertie en énumération DemiJournee, ou -1 en cas d'erreur.
 */
DemiJournee lireDemiJournee(const char *token) {
    return (strcmp(token, "am") == 0) ? MATIN :
           (strcmp(token, "pm") == 0) ? APRES_MIDI : -1;
}



/**
 * @ingroup Fonctions_gestion
 * @brief Calcule le jour courant en fonction des absences enregistrées.
 * 
 * Cette fonction retourne le jour le plus avancé trouvé dans les absences, que ce soit 
 * la date d'une absence ou la date d'un justificatif soumis.
 * 
 * @param absences Tableau d'absences à examiner.
 * @param nbAbsences Nombre d'absences dans le tableau.
 * @return int Le jour le plus avancé trouvé.
 */
int calculerJourCourant(Absence *absences, int nbAbsences) {
    int maxJour = 1;
    for (int i = 0; i < nbAbsences; i++) {
        if (absences[i].jour > maxJour) maxJour = absences[i].jour;
        if (absences[i].jourJustificatif > maxJour) maxJour = absences[i].jourJustificatif;
    }
    return maxJour;
}


/**
 * @ingroup Fonctions_gestion
 * @brief Compare deux absences par date et demi-journée pour le tri.
 * 
 * Cette fonction compare deux absences en fonction de la date et de la demi-journée.
 * Utilisée pour trier la liste des absences.
 * 
 * @param a Pointeur vers le premier pointeur d'absence.
 * @param b Pointeur vers le second pointeur d'absence.
 * @return int Différence basée sur la date et la demi-journée.
 */
int comparerAbsencesParDate(const void *a, const void *b) {
    Absence *absA = *(Absence**)a;
    Absence *absB = *(Absence**)b;
    if (absA->jour != absB->jour) {
        return absA->jour - absB->jour;
    } else if (absA->demiJournee != absB->demiJournee) {
        return absA->demiJournee - absB->demiJournee;
    } else {
        return absA->id - absB->id;
    }
}


/**
 * @ingroup Fonctions_gestion
 * @brief Compare deux absences pour le tri par étudiant et date.
 * 
 * Cette fonction compare deux absences en fonction de l'identifiant de l'étudiant
 * et de la date. Utilisée pour trier la liste des absences.
 * 
 * @param a Pointeur vers le premier pointeur d'absence.
 * @param b Pointeur vers le second pointeur d'absence.
 * @return int Différence basée sur l'identifiant de l'étudiant et la date.
 */
int comparerAbsences(const void *a, const void *b) {
    Absence *absA = *(Absence**)a;
    Absence *absB = *(Absence**)b;

    if (absA->etudiantId != absB->etudiantId) {
        return absA->etudiantId - absB->etudiantId;
    } else {
        return absA->jour - absB->jour;
    }
}


/********************************************************************************
 *                             [Gestion des Étudiants]                          *
 *                                                                              *
 *  Cette section regroupe toutes les fonctions liées à la gestion des          *
 *  étudiants. Cela inclut l'inscription, la recherche, et l'affichage des      *
 *  étudiants, ainsi que le calcul des statistiques liées aux absences.         *
 ********************************************************************************/


/**
 * @ingroup Fonctions_gestion
 * @brief Inscrit un étudiant dans le système.
 * 
 * Cette fonction permet d'inscrire un nouvel étudiant dans le tableau des étudiants.
 * Elle vérifie si l'étudiant n'est pas déjà inscrit et si la limite maximale d'étudiants n'est pas atteinte.
 * 
 * @param etudiants Tableau d'étudiants.
 * @param nbEtudiants Pointeur vers le nombre actuel d'étudiants.
 * @param nom Nom de l'étudiant à inscrire.
 * @param groupe Groupe de l'étudiant.
 */
void inscrireEtudiant(Etudiant *etudiants, int *nbEtudiants, const char *nom, int groupe) {
    if (*nbEtudiants >= MAX_ETUDIANTS) {
        afficherErreur("Limite d'etudiants atteinte.");
        return;
    }

    for (int i = 0; i < *nbEtudiants; i++) {
        if (strcmp(etudiants[i].nom, nom) == 0 && etudiants[i].groupe == groupe) {
            afficherErreur("Nom incorrect");
            return;
        }
    }

    Etudiant *nouvelEtudiant = &etudiants[*nbEtudiants];
    nouvelEtudiant->id = *nbEtudiants + 1;
    strcpy(nouvelEtudiant->nom, nom);
    nouvelEtudiant->groupe = groupe;
    nouvelEtudiant->nbAbsencesNonJustifiees = 0;

    (*nbEtudiants)++;
    printf("Inscription enregistree (%d)\n", nouvelEtudiant->id);
}

/**
 * @brief Trouve un étudiant par son identifiant.
 * 
 * Cette fonction recherche un étudiant dans un tableau d'étudiants
 * en fonction de son identifiant unique.
 * 
 * @param etudiants Tableau d'étudiants inscrits.
 * @param nbEtudiants Nombre total d'étudiants inscrits.
 * @param etudiantId Identifiant unique de l'étudiant recherché.
 * @return Pointeur vers l'étudiant trouvé, ou NULL si aucun étudiant ne correspond.
 */
Etudiant* trouverEtudiantParId(const Etudiant *etudiants, int nbEtudiants, int etudiantId) {
    for (int i = 0; i < nbEtudiants; i++) {
        if (etudiants[i].id == etudiantId) {
            return (Etudiant *)&etudiants[i];
        }
    }
    return NULL;
}

/**
 * @brief Calcule et met à jour les absences pour chaque étudiant.
 * 
 * Cette fonction permet de calculer et mettre à jour le nombre total
 * d'absences ainsi que les absences non justifiées pour chaque étudiant
 * en fonction du jour courant.
 * 
 * @param etudiants Tableau des étudiants inscrits.
 * @param nbEtudiants Nombre d'étudiants inscrits.
 * @param absences Tableau des absences enregistrées.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param jourCourant Jour actuel du système (utilisé pour la vérification des absences).
 */
void calculerAbsencesEtudiants(Etudiant *etudiants, int nbEtudiants, const Absence *absences, int nbAbsences, int jourCourant) {
    // Réinitialiser les compteurs d'absences
    for (int i = 0; i < nbEtudiants; i++) {
        etudiants[i].nbAbsencesNonJustifiees = 0;
        etudiants[i].nbTotalAbsences = 0;
    }

    // Calculer les absences pour chaque étudiant
    for (int i = 0; i < nbAbsences; i++) {
        if (absences[i].jour <= jourCourant) {
            Etudiant *etudiant = trouverEtudiantParId(etudiants, nbEtudiants, absences[i].etudiantId);
            if (etudiant != NULL) {
                etudiant->nbTotalAbsences++;
                StatutAbsence statut = determinerStatutAbsence(&absences[i], jourCourant);
                if (statut == NON_JUSTIFIEE) {
                    etudiant->nbAbsencesNonJustifiees++;
                }
            }
        }
    }
}

/********************************************************************************
 *                            [Gestion des Absences]                            *
 *                                                                              *
 *  Cette section regroupe les fonctions permettant de gérer les absences des   *
 *  étudiants, y compris l'enregistrement des absences, la validation des       *
 *  justificatifs, et la mise à jour des statuts.                               *
 ********************************************************************************/


/**
 * @ingroup Fonctions_gestion
 * @brief Enregistre une absence pour un étudiant donné.
 * 
 * Cette fonction permet d'enregistrer une absence pour un étudiant à une date spécifique
 * et pour une demi-journée. Elle vérifie la validité des paramètres avant d'ajouter l'absence.
 * 
 * @param absences Tableau d'absences.
 * @param nbAbsences Pointeur vers le nombre actuel d'absences.
 * @param etudiants Tableau des étudiants inscrits.
 * @param nbEtudiants Nombre d'étudiants inscrits.
 * @param etudiantId Identifiant de l'étudiant concerné.
 * @param jour Jour de l'absence.
 * @param demiJournee Demi-journée (matin ou après-midi) de l'absence.
 */
void enregistrerAbsence(Absence *absences, int *nbAbsences, const Etudiant *etudiants, int nbEtudiants, int etudiantId, int jour, DemiJournee demiJournee) {

    if (demiJournee == (DemiJournee)-1) {
        afficherErreur("Demi-journee incorrecte");
        return;
    }
    

    Etudiant *etudiant = trouverEtudiantParId(etudiants, nbEtudiants, etudiantId);
    if (etudiantId <= 0 || (etudiant = trouverEtudiantParId(etudiants, nbEtudiants, etudiantId)) == NULL) {
        afficherErreur("Identifiant incorrect");
        return;
    }


    if (jour < 1 || jour > NB_JOURS_SEMESTRE) {
        afficherErreur("Date incorrecte");
        return;
    }

    for (int i = 0; i < *nbAbsences; i++) {
        if (absences[i].etudiantId == etudiantId && absences[i].jour == jour && absences[i].demiJournee == demiJournee) {
            afficherErreur("Absence deja connue");
            return;
        }
    }

    Absence *nouvelleAbsence = &absences[*nbAbsences];
    nouvelleAbsence->id = *nbAbsences + 1;
    nouvelleAbsence->etudiantId = etudiantId;
    nouvelleAbsence->jour = jour;
    nouvelleAbsence->demiJournee = demiJournee;
    nouvelleAbsence->statut = EN_ATTENTE_JUSTIFICATIF;
    nouvelleAbsence->justificatif[0] = '\0';
    nouvelleAbsence->jourJustificatif = 0;

    (*nbAbsences)++;
    printf("Absence enregistree [%d]\n", nouvelleAbsence->id);
}

/**
 * @brief Trouve une absence par son identifiant.
 * 
 * Cette fonction recherche une absence spécifique dans un tableau d'absences
 * en utilisant l'identifiant unique de l'absence.
 * 
 * @param absences Tableau des absences enregistrées.
 * @param nbAbsences Nombre total d'absences enregistrées.
 * @param absenceId Identifiant unique de l'absence recherchée.
 * @return Pointeur vers l'absence trouvée, ou NULL si aucune absence ne correspond.
 */
Absence* trouverAbsenceParId(const Absence *absences, int nbAbsences, int absenceId) {
    if (absenceId <= 0) {
        return NULL;
    }
    for (int i = 0; i < nbAbsences; i++) {
        if (absences[i].id == absenceId) {
            return (Absence *)&absences[i];
        }
    }
    return NULL;
}

/**
 * @ingroup Fonctions_gestion
 * @brief Détermine le statut d'une absence.
 * 
 * Cette fonction détermine le statut d'une absence (en attente, justifiée, non justifiée) 
 * en fonction de la date du justificatif et du jour courant.
 * 
 * @param absence Pointeur vers la structure Absence à évaluer.
 * @param jourCourant Jour actuel du système (utilisé pour la vérification).
 * @return StatutAbsence Statut déterminé pour l'absence.
 */
StatutAbsence determinerStatutAbsence(const Absence *absence, int jourCourant) {
    if (absence->jour > jourCourant) {
        return EN_ATTENTE_JUSTIFICATIF;
    }

    int delaiDeposeJustificatif = absence->jour + DELAI_JUSTIFICATIF;
    int justificatifSoumis = (absence->jourJustificatif > 0 && absence->jourJustificatif <= jourCourant);
    int delaiDepasse = (jourCourant > delaiDeposeJustificatif);

    if (!justificatifSoumis) {
        return delaiDepasse ? NON_JUSTIFIEE : EN_ATTENTE_JUSTIFICATIF;
    } else {
        if (absence->jourJustificatif <= delaiDeposeJustificatif) {
            return (absence->statut == JUSTIFIEE || absence->statut == NON_JUSTIFIEE) ? absence->statut : EN_ATTENTE_VALIDATION;
        } else {
            return NON_JUSTIFIEE;
        }
    }
}


/**
 * @ingroup Fonctions_gestion
 * @brief Compare deux étudiants pour le tri.
 * 
 * Cette fonction compare deux étudiants en fonction de leur groupe et de leur nom.
 * Utilisée pour trier la liste des étudiants.
 * 
 * @param a Pointeur vers le premier étudiant.
 * @param b Pointeur vers le second étudiant.
 * @return int Différence entre les groupes ou comparaison des noms.
 */
int comparerEtudiants(const void *a, const void *b) {
    const Etudiant *etudiantA = a;
    const Etudiant *etudiantB = b;
    int groupeDiff = etudiantA->groupe - etudiantB->groupe;
    return (groupeDiff != 0) ? groupeDiff : strcmp(etudiantA->nom, etudiantB->nom);
}


/*********************************************************************************
 *                            [Gestion des Justificatifs]                        *
 *                                                                               *
 *  Cette section regroupe les fonctions permettant de valider les justificatifs *
 *  soumis pour les absences. Elle permet de valider ou de rejeter un            *
 *  justificatif en fonction de l'absence associée et du                         *
 *  code de validation fourni.                                                   *
 ********************************************************************************/

/**
 * @ingroup Fonctions_gestion
 * @brief Enregistre un justificatif pour une absence.
 * 
 * Cette fonction permet d'enregistrer un justificatif pour une absence existante.
 * Elle vérifie la validité de l'absence et la date de soumission du justificatif.
 * 
 * @param absences Tableau des absences.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param absenceId Identifiant de l'absence concernée.
 * @param jour Jour de soumission du justificatif.
 * @param justificatif Texte du justificatif.
 */
void enregistrerJustificatif(Absence *absences, int nbAbsences, int absenceId, int jour, const char *justificatif) {
    Absence *absence = trouverAbsenceParId(absences, nbAbsences, absenceId);
    if (absence == NULL) {
        afficherErreur("Identifiant incorrect");
        return;
    }

    if (jour < absence->jour) {
        afficherErreur("Date incorrecte");
        return;
    }

    if (strlen(absence->justificatif) > 0) {
        afficherErreur("Justificatif deja connu");
        return;
    }

    strcpy(absence->justificatif, justificatif);
    absence->jourJustificatif = jour;

    printf("Justificatif enregistre\n");
}

/**
 * @ingroup Fonctions_gestion
 * @brief Valide un justificatif pour une absence.
 * 
 * Cette fonction valide un justificatif soumis pour une absence donnée.
 * Elle vérifie que le statut de l'absence est approprié pour validation.
 * 
 * @param absences Tableau des absences.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param absenceId Identifiant de l'absence à valider.
 * @param code Code de validation (ok pour justifiée, ko pour non justifiée).
 * @param jourCourant Jour actuel du système (utilisé pour la vérification).
 */
void validerAbsence(Absence *absences, int nbAbsences, int absenceId, const char *code, int jourCourant) {
    Absence *absence = trouverAbsenceParId(absences, nbAbsences, absenceId);
    if (absence == NULL) {
        afficherErreur("Identifiant incorrect");
        return;
    }

    if (absence->statut == JUSTIFIEE || absence->statut == NON_JUSTIFIEE) {
        afficherErreur("Validation deja connue");
        return;
    }

    StatutAbsence statut = determinerStatutAbsence(absence, jourCourant);
    if (statut != EN_ATTENTE_VALIDATION) {
        afficherErreur("Identifiant incorrect");
        return;
    }

    if (strcmp(code, "ok") == 0) {
        absence->statut = JUSTIFIEE;
    } else if (strcmp(code, "ko") == 0) {
        absence->statut = NON_JUSTIFIEE;
    } else {
        afficherErreur("Code incorrect");
        return;
    }

    printf("Validation enregistree\n");
}

/********************************************************************************
 *                                [Affichage]                                   *
 *                                                                              *
 *  Cette section contient toutes les fonctions d'affichage pour présenter les  *
 *  informations sur les étudiants et leurs absences. Les fonctions ici         *
 *  permettent d'afficher les listes d'étudiants, les détails des absences,     *
 *  et d'autres statistiques importantes.                                       *
 ********************************************************************************/

/**
 * @brief Affiche un message d'erreur.
 * 
 * Cette fonction affiche un message d'erreur standardisé, ce qui
 * permet de centraliser et d'uniformiser les messages d'erreurs dans le programme.
 * 
 * @param message Chaîne de caractères du message d'erreur à afficher.
 */
void afficherErreur(const char *message) {
    printf("%s\n", message);
}


/**
 * @ingroup Fonctions_gestion
 * @brief Affiche les détails d'un étudiant et ses absences.
 * 
 * Cette fonction affiche les informations d'un étudiant spécifique ainsi que ses absences,
 * triées par statut et par date. Elle regroupe les absences en fonction de leur statut.
 * 
 * @param etudiants Tableau des étudiants.
 * @param nbEtudiants Nombre d'étudiants inscrits.
 * @param absences Tableau des absences enregistrées.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param etudiantId Identifiant de l'étudiant à afficher.
 * @param jourCourant Jour actuel du système (utilisé pour la vérification des absences).
 */
void afficherEtudiant(Etudiant *etudiants, int nbEtudiants, Absence *absences, int nbAbsences, int etudiantId, int jourCourant) {
    if (jourCourant < JOUR_DEBUT_SEMESTRE) {
        afficherErreur("Date incorrecte");
        return;
    }

    Etudiant *etudiant = trouverEtudiantParId(etudiants, nbEtudiants, etudiantId);
    if (etudiant == NULL) {
        afficherErreur("Identifiant incorrect");
        return;
    }

    // Calcul du nombre total d'absences
    etudiant->nbTotalAbsences = 0;
    for (int i = 0; i < nbAbsences; i++) {
        if (absences[i].etudiantId == etudiant->id && absences[i].jour <= jourCourant) {
            etudiant->nbTotalAbsences++;
        }
    }

    // Affichage des informations de l'étudiant avec le formatage exact
    printf("(%d) %s %d %d\n", etudiant->id, etudiant->nom, etudiant->groupe, etudiant->nbTotalAbsences);

    // Initialisation des listes d'absences par statut
    Absence* enAttenteJustificatif[MAX_ABSENCES];
    int nbEnAttenteJustificatif = 0;
    Absence* enAttenteValidation[MAX_ABSENCES];
    int nbEnAttenteValidation = 0;
    Absence* justifiees[MAX_ABSENCES];
    int nbJustifiees = 0;
    Absence* nonJustifiees[MAX_ABSENCES];
    int nbNonJustifiees = 0;

    // Tri des absences en fonction de leur statut
    for (int i = 0; i < nbAbsences; i++) {
        Absence *absence = &absences[i];
        if (absence->etudiantId == etudiant->id && absence->jour <= jourCourant) {
            StatutAbsence statut = determinerStatutAbsence(absence, jourCourant);

            switch (statut) {
                case EN_ATTENTE_JUSTIFICATIF:
                    enAttenteJustificatif[nbEnAttenteJustificatif++] = absence;
                    break;
                case EN_ATTENTE_VALIDATION:
                    enAttenteValidation[nbEnAttenteValidation++] = absence;
                    break;
                case JUSTIFIEE:
                    justifiees[nbJustifiees++] = absence;
                    break;
                case NON_JUSTIFIEE:
                    nonJustifiees[nbNonJustifiees++] = absence;
                    break;
            }
        }
    }

    // Affichage des absences selon leur statut
    if (nbEnAttenteJustificatif > 0) {
        qsort(enAttenteJustificatif, nbEnAttenteJustificatif, sizeof(Absence*), comparerAbsencesParDate);
        printf("- En attente justificatif\n");
        for (int i = 0; i < nbEnAttenteJustificatif; i++) {
            Absence *absence = enAttenteJustificatif[i];
            printf("  [%d] %d/%s\n", absence->id, absence->jour,
                   absence->demiJournee == MATIN ? "am" : "pm");
        }
    }

    if (nbEnAttenteValidation > 0) {
        qsort(enAttenteValidation, nbEnAttenteValidation, sizeof(Absence*), comparerAbsencesParDate);
        printf("- En attente validation\n");
        for (int i = 0; i < nbEnAttenteValidation; i++) {
            Absence *absence = enAttenteValidation[i];
            printf("  [%d] %d/%s (%s)\n", absence->id, absence->jour,
                   absence->demiJournee == MATIN ? "am" : "pm", absence->justificatif);
        }
    }

    if (nbJustifiees > 0) {
        qsort(justifiees, nbJustifiees, sizeof(Absence*), comparerAbsencesParDate);
        printf("- Justifiees\n");
        for (int i = 0; i < nbJustifiees; i++) {
            Absence *absence = justifiees[i];
            printf("  [%d] %d/%s (%s)\n", absence->id, absence->jour,
                   absence->demiJournee == MATIN ? "am" : "pm", absence->justificatif);
        }
    }

    if (nbNonJustifiees > 0) {
        qsort(nonJustifiees, nbNonJustifiees, sizeof(Absence*), comparerAbsencesParDate);
        printf("- Non-justifiees\n");
        for (int i = 0; i < nbNonJustifiees; i++) {
            Absence *absence = nonJustifiees[i];
            printf("  [%d] %d/%s", absence->id, absence->jour,
                   absence->demiJournee == MATIN ? "am" : "pm");
            if (absence->jourJustificatif > 0 && absence->jourJustificatif <= jourCourant) {
                printf(" (%s)", absence->justificatif);
            }
            printf("\n");
        }
    }
}

/**
 * @ingroup Fonctions_gestion
 * @brief Affiche la liste des étudiants avec leurs statistiques d'absences.
 * 
 * Cette fonction affiche la liste des étudiants avec le nombre total
 * d'absences et les absences non justifiées. La liste est triée par groupe
 * et par nom d'étudiant.
 * 
 * @param etudiants Tableau des étudiants inscrits.
 * @param nbEtudiants Nombre d'étudiants inscrits.
 * @param absences Tableau des absences enregistrées.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param jourCourant Jour actuel du système (utilisé pour la vérification des absences).
 */
void afficherEtudiants(Etudiant *etudiants, int nbEtudiants, const Absence *absences, int nbAbsences, int jourCourant) {
    if (jourCourant < JOUR_DEBUT_SEMESTRE) {
        afficherErreur("Date incorrecte");
        return;
    }

    if (nbEtudiants == 0) {
        afficherErreur("Aucun inscrit");
        return;
    }

    calculerAbsencesEtudiants(etudiants, nbEtudiants, absences, nbAbsences, jourCourant);

    // Trier les étudiants
    qsort(etudiants, nbEtudiants, sizeof(Etudiant), comparerEtudiants);

    // Afficher les étudiants avec le formatage exact
    for (int i = 0; i < nbEtudiants; i++) {
        printf("(%d) %s %d %d\n",
            etudiants[i].id,
            etudiants[i].nom,
            etudiants[i].groupe,
            etudiants[i].nbTotalAbsences);
    }
}


/**
 * @ingroup Fonctions_gestion
 * @brief Affiche toutes les absences en attente de validation.
 * 
 * Cette fonction parcourt la liste des absences et affiche celles
 * qui sont en attente de validation, en les triant par date.
 * 
 * @param absences Tableau des absences.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param etudiants Tableau des étudiants inscrits.
 * @param nbEtudiants Nombre d'étudiants inscrits.
 * @param jourCourant Jour actuel du système (utilisé pour la vérification).
 */
void afficherAbsencesEnAttente(Absence *absences, int nbAbsences, Etudiant *etudiants, int nbEtudiants, int jourCourant) {
    Absence *absencesEnAttente[MAX_ABSENCES];
    int nbEnAttente = 0;

    for (int i = 0; i < nbAbsences; i++) {
        Absence *absence = &absences[i];
        if (absence->jourJustificatif > 0 && absence->jourJustificatif <= jourCourant) {
            StatutAbsence statut = determinerStatutAbsence(absence, jourCourant);
            if (statut == EN_ATTENTE_VALIDATION) {
                absencesEnAttente[nbEnAttente++] = absence;
            }
        }
    }

    if (nbEnAttente == 0) {
        printf("Aucune validation en attente\n");
        return;
    }

    qsort(absencesEnAttente, nbEnAttente, sizeof(Absence*), comparerAbsences);

    for (int i = 0; i < nbEnAttente; i++) {
        Absence *absence = absencesEnAttente[i];
        Etudiant *etudiant = NULL;
        for (int j = 0; j < nbEtudiants; j++) {
            if (etudiants[j].id == absence->etudiantId) {
                etudiant = &etudiants[j];
                break;
            }
        }
        if (etudiant != NULL) {
            printf("[%d] (%d) %s %d %d/%s (%s)\n",
                absence->id, etudiant->id, etudiant->nom, etudiant->groupe,
                absence->jour, absence->demiJournee == MATIN ? "am" : "pm",
                absence->justificatif);
        }
    }
}


/**
 * @brief Affiche les détails d'une absence.
 * 
 * Cette fonction affiche les informations d'une absence, y compris l'ID, 
 * le jour et si c'est le matin ou l'après-midi. Si le paramètre 
 * afficherJustificatif est vrai et qu'un justificatif est présent, 
 * il sera également affiché.
 * 
 * @param absence Pointeur vers une structure Absence contenant les détails de l'absence.
 * @param afficherJustificatif Indicateur pour afficher ou non le justificatif.
 */
void afficherAbsence(const Absence *absence, int afficherJustificatif) {
    printf("  [%d] %d/%s", absence->id, absence->jour,
           absence->demiJournee == MATIN ? "am" : "pm");

    if (afficherJustificatif && strlen(absence->justificatif) > 0) {
        printf(" (%s)", absence->justificatif);
    }
    printf("\n");
}

/**
 * @ingroup Fonctions_gestion
 * @brief Affiche la liste des étudiants défaillants.
 * 
 * Cette fonction identifie les étudiants ayant dépassé la limite
 * d'absences non justifiées et affiche leurs informations.
 * 
 * @param etudiants Tableau des étudiants inscrits.
 * @param nbEtudiants Nombre d'étudiants inscrits.
 * @param absences Tableau des absences enregistrées.
 * @param nbAbsences Nombre d'absences enregistrées.
 * @param jourCourant Jour actuel du système (utilisé pour la vérification des absences).
 */
void afficherDefaillants(Etudiant *etudiants, int nbEtudiants, const Absence *absences, int nbAbsences, int jourCourant) {
    int anyDefaillant = 0;

    if (jourCourant < 1) {
        afficherErreur("Date incorrecte");
        return;
    }

    // Initialiser les compteurs d'absences
    for (int i = 0; i < nbEtudiants; i++) {
        etudiants[i].nbAbsencesNonJustifiees = 0;
        etudiants[i].nbTotalAbsences = 0;
    }

    calculerAbsencesEtudiants(etudiants, nbEtudiants, absences, nbAbsences, jourCourant);

    for (int i = 0; i < nbEtudiants; i++) {
        if (etudiants[i].nbAbsencesNonJustifiees >= LIMITE_ABSENCES_NON_JUSTIFIEES) {
            printf("(%d) %-13s %2d %d\n",
                   etudiants[i].id,
                   etudiants[i].nom,
                   etudiants[i].groupe,
                   etudiants[i].nbTotalAbsences);
            anyDefaillant = 1;
        }
    }

    if (!anyDefaillant) {
        afficherErreur("Aucun defaillant");
    }
}

/********************************************************************************
 *                            [Commandes Utilisateur]                           *
 *                                                                              *
 *  Cette section regroupe les fonctions qui interprètent et exécutent les      *
 *  commandes saisies par l'utilisateur. Chaque commande permet d'interagir     *
 *  avec le système pour gérer les inscriptions, absences, et autres opérations.*
 ********************************************************************************/


/**
 * @ingroup Fonctions_gestion
 * @brief Quitte le programme proprement.
 * 
 * Cette fonction termine l'exécution du programme en appelant la fonction `exit`.
 */
void quitterProgramme() {
    exit(0);
}

/**
 * @brief Convertit une commande en code enuméré.
 * 
 * Cette fonction permet de convertir une commande utilisateur sous forme
 * de chaîne de caractères en un code de commande spécifique.
 * 
 * @param cmd Chaîne de caractères représentant la commande.
 * @return CodeCommande Code correspondant à la commande, CMD_INCONNUE si non reconnu.
 */
CodeCommande obtenirCodeCommande(const char *cmd) {
    if (strcmp(cmd, "inscription") == 0) return CMD_INSCRIPTION;
    if (strcmp(cmd, "absence") == 0) return CMD_ABSENCE;
    if (strcmp(cmd, "justificatif") == 0) return CMD_JUSTIFICATIF;
    if (strcmp(cmd, "validation") == 0) return CMD_VALIDATION;
    if (strcmp(cmd, "etudiant") == 0) return CMD_ETUDIANT;
    if (strcmp(cmd, "etudiants") == 0) return CMD_ETUDIANTS;
    if (strcmp(cmd, "validations") == 0) return CMD_VALIDATIONS;
    if (strcmp(cmd, "defaillants") == 0) return CMD_DEFAILLANTS;
    if (strcmp(cmd, "exit") == 0) return CMD_EXIT;
    return CMD_INCONNUE;
}


/**
 * @ingroup Fonctions_gestion
 * @brief Traite une commande utilisateur.
 * 
 * Cette fonction interprète et exécute une commande saisie par l'utilisateur,
 * permettant la gestion des inscriptions, des absences, des justificatifs et
 * des validations.
 * 
 * @param commande Chaîne de caractères représentant la commande utilisateur.
 * @param etudiants Tableau des étudiants inscrits.
 * @param absences Tableau des absences enregistrées.
 * @param nbEtudiants Pointeur vers le nombre d'étudiants inscrits.
 * @param nbAbsences Pointeur vers le nombre d'absences enregistrées.
 */
void traiterCommande(char *commande, Etudiant *etudiants, Absence *absences, int *nbEtudiants, int *nbAbsences) {
    char *cmd = strtok(commande, " ");
    if (cmd == NULL) {
        afficherErreur("Commande inconnue");
        return;
    }

    CodeCommande codeCmd = obtenirCodeCommande(cmd);

    switch (codeCmd) {
        case CMD_INSCRIPTION: {
            char *nom = strtok(NULL, " ");
            int groupe = lireEntier(strtok(NULL, " "));
            inscrireEtudiant(etudiants, nbEtudiants, nom, groupe);
            break;
        }

        case CMD_ABSENCE: {
            char *etudiantIdStr = strtok(NULL, " ");
            char *jourStr = strtok(NULL, " ");
            char *demiJourneeStr = strtok(NULL, " ");

            int etudiantId = lireEntier(etudiantIdStr);
            int jour = lireEntier(jourStr);
            DemiJournee demiJournee = lireDemiJournee(demiJourneeStr);

            enregistrerAbsence(absences, nbAbsences, etudiants, *nbEtudiants, etudiantId, jour, demiJournee);
            break;
        }

        case CMD_JUSTIFICATIF: {
            int absenceId = lireEntier(strtok(NULL, " "));
            int jour = lireEntier(strtok(NULL, " "));
            char *justificatif = strtok(NULL, "");
            if (justificatif == NULL) {
                justificatif = "";
            } else {
                while (*justificatif == ' ') {
                    justificatif++;
                }
            }
            enregistrerJustificatif(absences, *nbAbsences, absenceId, jour, justificatif);
            break;
        }

        case CMD_VALIDATION: {
            int absenceId = lireEntier(strtok(NULL, " "));
            char *code = strtok(NULL, " ");
            int jourCourant = calculerJourCourant(absences, *nbAbsences);
            validerAbsence(absences, *nbAbsences, absenceId, code, jourCourant);
            break;
        }

        case CMD_ETUDIANT: {
            int etudiantId = lireEntier(strtok(NULL, " "));
            int jour = lireEntier(strtok(NULL, " "));
            afficherEtudiant(etudiants, *nbEtudiants, absences, *nbAbsences, etudiantId, jour);
            break;
        }

        case CMD_ETUDIANTS: {
            int jour = lireEntier(strtok(NULL, " "));
            afficherEtudiants(etudiants, *nbEtudiants, absences, *nbAbsences, jour);
            break;
        }

        case CMD_VALIDATIONS: {
            int jourCourant = calculerJourCourant(absences, *nbAbsences);
            afficherAbsencesEnAttente(absences, *nbAbsences, etudiants, *nbEtudiants, jourCourant);
            break;
        }

        case CMD_DEFAILLANTS: {
            int jour = lireEntier(strtok(NULL, " "));
            afficherDefaillants(etudiants, *nbEtudiants, absences, *nbAbsences, jour);
            break;
        }

        case CMD_EXIT:
            quitterProgramme();
            break;

        default:
            afficherErreur("Commande inconnue");
            break;
    }
}

/********************************************************************************
 *                              [Point d'Entrée]                                *
 *                                                                              *
 *  Le point d'entrée principal du programme. Cette fonction initialise les     *
 *  structures de données et gère la boucle principale d'attente des commandes  *
 *  utilisateur. Le programme reste en boucle tant qu'une commande valide ne    *
 *  demande pas la fermeture via "exit".                                        *
 ********************************************************************************/
/**
 * @ingroup Fonctions_gestion
 * @brief Point d'entrée principal du programme.
 * 
 * Cette fonction initialise le programme, gère la boucle principale
 * d'attente des commandes utilisateur et traite les données concernant
 * les étudiants et leurs absences.
 * 
 * Le programme reste en boucle tant qu'une commande valide n'a pas
 * demandé la fermeture via "exit".
 * 
 * @return int Code de retour du programme (0 en cas de succès).
 */
int main() {
    Etudiant etudiants[MAX_ETUDIANTS];
    Absence absences[MAX_ABSENCES];
    int nbEtudiants = 0;
    int nbAbsences = 0;

    char commande[MAX_COMMANDE];
    
    
    while (1) {
        lireLigne(commande, MAX_COMMANDE);
        traiterCommande(commande, etudiants, absences, &nbEtudiants, &nbAbsences);
    }

    return 0;
}

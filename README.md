> [!WARNING]
> This is a school project for the IUT Rives de Seine. The subject is not available as requested by my professor. This is just a project that we had to make to enter it in 1st year of CS.

# Gestion des Absences des Étudiants en BUT

## Table des Matières

- [Introduction](#introduction)
- [Installation](#installation)
- [Utilisation](#utilisation)
  - [Commandes Disponibles](#commandes-disponibles)
    - [C0 - Sortie du Programme](#c0---sortie-du-programme)
    - [C1 - Inscription](#c1---inscription)
    - [C2 - Enregistrement d'une Absence](#c2---enregistrement-dune-absence)
    - [C3 - Liste des Étudiants](#c3---liste-des-étudiants)
    - [C4 - Dépôt d'un Justificatif](#c4---dépôt-dun-justificatif)
    - [C5 - Liste des Absences en Attente de Validation](#c5---liste-des-absences-en-attente-de-validation)
    - [C6 - Validation/Invalidation d'un Justificatif](#c6---validationinvalidation-dun-justificatif)
    - [C7 - Situation d'un Étudiant](#c7---situation-dun-étudiant)
    - [C8 - Liste des Étudiants Défaillants](#c8---liste-des-étudiants-défaillants)
- [Exemples](#exemples)
- [Limites](#limites)
- [Licence](#licence)

---

## Introduction

> [!NOTE]
> 
> Ce projet est une application en ligne de commande permettant de gérer les absences des étudiants inscrits en BUT (Bachelor Universitaire de Technologie). Les étudiants doivent respecter certaines règles concernant leurs absences, et l'application permet de suivre ces absences, de gérer les justificatifs, et de déterminer les étudiants en situation de défaillance.

L'application interprète diverses commandes permettant d'inscrire des étudiants, d'enregistrer des absences, de soumettre et valider des justificatifs, et de consulter les informations liées aux absences.

## Installation

> [!IMPORTANT]
> 
> Assurez-vous d'avoir un compilateur C installé sur votre système (comme GCC) pour pouvoir compiler le projet.

Pour compiler le programme :

1. Clonez le dépôt du projet ou téléchargez le fichier `projet.c`.

2. Compilez le code source avec la commande suivante :

```bash
gcc projet.c -o gestion_absences
```

3. Le programme exécutable `gestion_absences` sera créé.

> [!TIP]
> 
> Si vous rencontrez des problèmes lors de la compilation, vérifiez que votre version de GCC est à jour.

## Utilisation

Pour lancer le programme, exécutez simplement le fichier compilé :

```bash
./gestion_absences
```

Le programme attend des commandes saisies par l'utilisateur via l'entrée standard (le terminal). Chaque commande doit respecter un format spécifique et se terminer par un retour à la ligne.

### Commandes Disponibles

#### C0 - Sortie du Programme

- **Syntaxe** : `exit`

- **Description** : Termine l'exécution du programme.

#### C1 - Inscription

- **Syntaxe** : `inscription <nom> <groupe>`

- **Paramètres**:
  - `<nom>` : Le nom de l'étudiant (maximum 30 caractères, sans espaces).
  - `<groupe>` : Le numéro de groupe de l'étudiant (entier).

- **Description** : Inscrit un nouvel étudiant dans le système.

- **Réponse du Programme** :
  - `Inscription enregistree (<id>)` : Si l'inscription est réussie. `<id>` est l'identifiant attribué à l'étudiant.
  - `Nom incorrect` : Si un étudiant portant le même nom est déjà inscrit dans le même groupe.

> [!NOTE]
> 
> Chaque étudiant inscrit reçoit un identifiant unique qui sera utilisé pour toutes les autres commandes.

#### C2 - Enregistrement d'une Absence

- **Syntaxe** : `absence <id_etudiant> <jour> <demi-journee>`

- **Paramètres**:
  - `<id_etudiant>` : L'identifiant de l'étudiant (attribué lors de l'inscription).
  - `<jour>` : Le numéro du jour (entier entre 1 et 40).
  - `<demi-journee>` : `am` pour le matin, `pm` pour l'après-midi.

- **Description** : Enregistre une absence pour un étudiant à une date donnée.

- **Réponse du Programme** :
  - `Absence enregistree [<id_absence>]` : Si l'enregistrement est réussi. `<id_absence>` est l'identifiant attribué à l'absence.
  - `Identifiant incorrect` : Si l'identifiant de l'étudiant est invalide.
  - `Date incorrecte` : Si le numéro de jour est invalide.
  - `Demi-journee incorrecte` : Si la demi-journée est invalide.
  - `Absence deja connue` : Si une absence pour ce créneau est déjà enregistrée.

> [!NOTE]
> 
> Une absence doit être enregistrée dans le respect des contraintes de date et de demi-journée pour éviter des erreurs.

#### C3 - Liste des Étudiants

- **Syntaxe** : `etudiants <jour_courant>`

- **Paramètres**:
  - `<jour_courant>` : Le numéro du jour actuel (entier supérieur ou égal à 1).

- **Description** : Affiche la liste des étudiants inscrits, avec leur nombre total d'absences jusqu'au jour courant.

- **Réponse du Programme** :
  - `Date incorrecte` : Si le jour courant est invalide.
  - `Aucun inscrit` : Si aucun étudiant n'est inscrit.
  - Liste des étudiants au format :
    ```
    (<id>) <nom> <groupe> <nb_absences>
    ```

- **Tri des Étudiants** :
  - Par ordre croissant de groupe.
  - Au sein d'un même groupe, par ordre alphabétique du nom.

> [!TIP]
> 
> Pour un affichage optimal, triez toujours les étudiants par groupe et par ordre alphabétique pour une meilleure visibilité.

#### C4 - Dépôt d'un Justificatif

- **Syntaxe** : `justificatif <id_absence> <jour> <texte_justificatif>`

- **Paramètres**:
  - `<id_absence>` : L'identifiant de l'absence.
  - `<jour>` : Le jour où le justificatif est déposé.
  - `<texte_justificatif>` : Une chaîne de caractères (maximum 50 caractères) expliquant l'absence.

- **Description** : Permet à un étudiant de déposer un justificatif pour une absence.

- **Réponse du Programme** :
  - `Identifiant incorrect` : Si l'identifiant de l'absence est invalide.
  - `Date incorrecte` : Si le jour de dépôt du justificatif précède la date de l'absence.
  - `Justificatif deja connu` : Si un justificatif a déjà été déposé pour cette absence.
  - `Justificatif enregistre` : Si le justificatif est enregistré avec succès.

> [!NOTE]
> 
> Un justificatif doit être déposé dans un délai de 3 jours après l'absence.

#### C5 - Liste des Absences en Attente de Validation

- **Syntaxe** : `validations`

- **Description** : Affiche la liste des absences pour lesquelles un justificatif a été déposé dans les temps et qui sont en attente de validation par le directeur des études.

- **Réponse du Programme** :
  - `Aucune validation en attente` : Si aucune absence n'est en attente de validation.
  - Liste des absences au format :
    ```
    [<id_absence>] (<id_etudiant>) <nom> <groupe> <jour>/<demi-journee> (<texte_justificatif>)
    ```

#### C6 - Validation/Invalidation d'un Justificatif

- **Syntaxe** : `validation <id_absence> <code>`

- **Paramètres**:
  - `<id_absence>` : L'identifiant de l'absence.
  - `<code>` : `ok` pour valider le justificatif, `ko` pour l'invalider.

- **Description** : Permet au directeur des études de valider ou invalider un justificatif déposé.

- **Réponse du Programme** :
  - `Validation enregistree` : Si la validation est enregistrée avec succès.
  - `Identifiant incorrect` : Si l'identifiant de l'absence est invalide ou si l'absence n'est pas en attente de validation.
  - `Code incorrect` : Si le code de validation n'est ni `ok` ni `ko`.
  - `Validation deja connue` : Si le justificatif a déjà été validé ou invalidé.

> [!WARNING]
> 
> Utilisez `ok` ou `ko` uniquement pour valider ou invalider un justificatif. Toute autre valeur sera considérée comme incorrecte.

#### C7 - Situation d'un Étudiant

- **Syntaxe** : `etudiant <id_etudiant> <jour_courant>`

- **Paramètres**:
  - `<id_etudiant>` : L'identifiant de l'étudiant.
  - `<jour_courant>` : Le numéro du jour actuel.

- **Description** : Affiche les informations détaillées d'un étudiant, y compris ses absences classées par statut.

- **Réponse du Programme** :
  - `Identifiant incorrect` : Si l'identifiant de l'étudiant est invalide.
  - `Date incorrecte` : Si le jour courant est invalide.
  - Informations de l'étudiant et ses absences, classées par statut :
    - En attente de justificatif.
    - En attente de validation.
    - Justifiées.
    - Non-justifiées.

#### C8 - Liste des Étudiants Défaillants

- **Syntaxe** : `defaillants <jour_courant>`

- **Paramètres**:
  - `<jour_courant>` : Le numéro du jour actuel.

- **Description** : Affiche la liste des étudiants ayant atteint ou dépassé la limite de 5 demi-journées d'absences non justifiées.

- **Réponse du Programme** :
  - `Date incorrecte` : Si le jour courant est invalide.
  - `Aucun defaillant` : Si aucun étudiant n'est défaillant.
  - Liste des étudiants défaillants au format :
    ```
    (<id>) <nom> <groupe> <nb_absences_total>
    ```

## Exemples

### Exemple de Session Complète

> 💬 **Exemple**
> 
> Voici un exemple complet utilisant toutes les commandes disponibles :
> 
> ```
> inscription Alice 101
> Inscription enregistree (1)
> inscription Bob 102
> Inscription enregistree (2)
> inscription Charlie 101
> Inscription enregistree (3)
> inscription Diana 103
> Inscription enregistree (4)
> inscription Eve 102
> Inscription enregistree (5)
> inscription Alice 101
> Nom incorrect
> absence 1 2 am
> Absence enregistree [1]
> absence 2 3 pm
> Absence enregistree [2]
> absence 3 1 am
> Absence enregistree [3]
> absence 4 5 pm
> Absence enregistree [4]
> absence 5 2 am
> Absence enregistree [5]
> absence 1 2 am
> Absence deja connue
> justificatif 1 4 Probleme de transport
> Justificatif enregistre
> justificatif 2 5 Maladie
> Justificatif enregistre
> justificatif 3 6 Visite chez le médecin
> Justificatif enregistre
> justificatif 5 3 Malade
> Justificatif enregistre
> justificatif 2 5 Maladie
> Justificatif deja connu
> validation 1 ok
> Validation enregistree
> validation 2 ko
> Validation enregistree
> validation 6 ko
> Identifiant incorrect
> validation 3 ko
> Validation enregistree
> validation 4 ok
> Validation enregistree
> etudiant 1 7
> (1) Alice 101 1
> - Justifiees
> [1] 2/am (Probleme de transport)
> etudiant 2 7
> (2) Bob 102 1
> - Non-justifiees
> [2] 3/pm (Maladie)
> etudiant 3 7
> (3) Charlie 101 1
> - Non-justifiees
> [3] 1/am (Visite chez le médecin)
> etudiant 4 7
> (4) Diana 103 1
> - Justifiees
> [4] 5/pm
> etudiant 5 7
> (5) Eve 102 1
> - En attente validation
> [5] 2/am (Malade)
> etudiants 7
> (1) Alice 101 1
> (3) Charlie 101 1
> (2) Bob 102 1
> (5) Eve 102 1
> (4) Diana 103 1
> validations
> [5] (5) Eve 102 2/am (Malade)
> defaillants 10
> Aucun defaillant
> exit
> ```

## Limites

> [!CAUTION]
> 
> Voici les limites actuelles du système :
> - **Nombre maximal d'étudiants** : 100
> - **Nombre maximal d'absences** : 200
> - **Longueur maximale du nom d'un étudiant** : 30 caractères
> - **Longueur maximale d'un justificatif** : 50 caractères
> - **Nombre de jours dans un semestre** : 40
> - **Délai pour soumettre un justificatif** : 3 jours après l'absence
> - **Limite d'absences non justifiées** : 5 demi-journées par semestre
> - Ces limites sont imposés par le projet.

## Licence

> [!TIP]
> 
> Ce projet est sous licence The Unlicense. Voir le fichier [LICENSE](LICENSE) pour plus de détails.

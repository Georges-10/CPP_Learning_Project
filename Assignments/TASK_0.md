# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Sur quelle touche faut-il appuyer pour ajouter un avion ?
* c
Comment faire pour quitter le programme ?
A quoi sert la touche 'F' ?
</br>**-la touche F sert à afficher en plein écrans**

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
** il vole et atterit à l'infini
Quelles informations s'affichent dans la console ?
**</br>-quand on lance un envion la console affiche c'est diffirent etats de vole**  
Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ? 
**</br>ils font la même chose mais à 4, il n'attirissent pas au même moment**
## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.
* AircraftType : Représente le type d'un avion et ses caractéristiques (vitesse aérienne, 
terrestre, acceleration etc...).
* Aircraft : Classe représentant un avion.
* AirportType: Class représentant le type d'aéroport indique des informations tels que la 
positions des terminaux, un ensemble de runway, la position du gateway et la crossing 
position.
* La classe Airport indique les informations essentielle de l'aéroport tel que son 
AirportType, sa position, ses terminaux et sa tour.
* Point2D et point 3D représentant des points en deuxième et troisième dimension.
* runway: class permettant de représentant une piste.
* Terminal: Cette classe permet d'assigner aux avions les opérations qu'ils devront 
effectuer.
* TowerSimulation: classe permettant d'initialiser l'aéroport, le lancer et d'assigner des 
actions via la ligne de commande.
* Tower: Tour de contrôle a des fonctions liés au terminal et aux avions.
* WayPoint : Classe héritant de Point3D, possède un Enum indiquant si le waypoint est 
terrestre aérien ou un terminal. Une queue de Waypoint est appelé WaypointQueue.


Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.
**<br/>Tower**
- WaypointQueue Tower::get_circle() const -> Retourne un ensemble de WayPoint de type 
wp_air.
- WaypointQueue Tower::get_instructions(Aircraft& aircraft) -> fonctions donnat des 
instructions à un avion en fonction de sa position courante. Si l'avion est sur un terminal la
tour lui trouve un chemin et le permet de finir son service pour en commencer un 
nouveau. Sinon l'avion est guidé vers un terminal s'il y en a un de disponible.
- void Tower::arrived_at_terminal(const Aircraft& aircraft) -> trouve si le aircraft est sur un 
terminal et le fait démarrer son service
**<br/>Aircraft**
- Constructeur 
- get_flight_num: getter du champs flight_number
- distance_to: permet de calculer la distance entre un point donné et l'avion
- display: dessine l'avion 
- move: permet de déplacer l'avion, si le waypoints est vide il prend des instructions 
auprès d'une tour de controle, sinon avance selon le prochain waypoint pour approcher de 
son objectif final.
**<br/>Airport**
- Constructeur 
- get_tower getter de l'attribut tower
- display affiche l'aeroport
- move lance la fonction move de tous les terminaux de l'aéroport (augmente la 
progression du service d'un avion)
Terminal
- Constructeur
- in_use : permet de savoir si l'avion courant n'est pas à null
- is_servicing : compare le service progress et SERVICE_CYCLES 
- assign_craft : assign un avion au terminal (current_aircraft = aircraft)
- start_service : démarre le service d'un avion s'il se trouve à proximité du terminal met la 
variable service progress à 0
- finish_service: met fin au service de l'avion si la fonction is_sevicing retourne faux et met 
l'avion courant à null


Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
Expliquez les intérêts de ce choix.
**</br>** 
* Plusieurs classes sont impliquées dans la génération du chemin d’un avion. 
Aircraft appel la fonction move qui fait lui même appel à la fonction de Tower 
get_instructions lorsqu’il n’y a pas de waypoints.
Il y a aussi un appel à reserve terminal de Airport qui permet a un terminal de se voir 
assigner un avion via Terminal::assign_craft(), cette fonction met à jour le chemin de 
l’avion en cas de réussite. 

* Enfin, pour représenter un chemin, une queue est utilisé car les nouveaux points doivent être ajouter à la fin pour pouvoir être récupérer en premier.**

## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.
* AircraftTypes (l.34) changer le 2eme paramètre de .05f à .2f

2) Identifiez quelle variable contrôle le framerate de la simulation.
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ?\
Ajoutez une nouvelle fonctionnalité au programme pour mettre le programme en pause, et qui ne passe pas par le framerate.
* La variable qui contrôle le framerate est GL::tick_per_frame, si on met tik_per_sec à 0 il y a
une division par zéro dans la fonction GL::timer.


3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
* C’est la variable SERVICES_CYCLES.


4) Lorsqu'un avion a décollé, il réattérit peu de temps après.
Faites en sorte qu'à la place, il soit retiré du programme.\
Indices :\
A quel endroit pouvez-vous savoir que l'avion doit être supprimé ?\
*  Aircraft::move
Pourquoi n'est-il pas sûr de procéder au retrait de l'avion dans cette fonction ?
*  parce que le seul qui own les avions est GL::move_queue
A quel endroit de la callstack pourriez-vous le faire à la place ?\
* GL::timer\
Que devez-vous modifier pour transmettre l'information de la première à la seconde fonction ?
*  un bool

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?
* L’ajout et la suppression des élémentsde la display queue se feront dans le constructeur 
et le destructeur de Displayable. On peut ensuite retirer les ajouts et les suppressions 
manuels

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.
* On va utiliser une map pour avoir une recherche et une suppression d’élément en temps 
constant.
## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?
* Seul, le Airport appel assign_craft.

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?
* Il n’est pas nécessaire de passer par une référence car cette fonction permet 
de mettre à jour le champs speed de l’avion. Pour éviter la copie du Point3D, on
pourrait directement mettre en paramètre, la valeur résultant du traitement de 
direction permettant de mettre à jour point 3D.
## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.

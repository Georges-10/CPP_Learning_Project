# Assertions et exceptions

## Objectif 1 - Crash des avions

Actuellement, quand un avion s'écrase, une exception de type `AircraftCrash` (qui est un alias de `std::runtime_error` déclaré dans `config.hpp`) est lancée.

1. Faites en sorte que le programme puisse continuer de s'exécuter après le crash d'un avion. Pour cela, remontez l'erreur jusqu'à un endroit approprié pour procéder à la suppression de cet avion (assurez-vous bien que plus personne ne référence l'avion une fois l'exception traitée). Vous afficherez également le message d'erreur de l'exception dans `cerr`.
2. Introduisez un compteur qui est incrémenté chaque fois qu'un avion s'écrase. Choisissez une touche du clavier qui n'a pas encore été utilisée (`m` par exemple ?) et affichez ce nombre dans la console lorsque l'utilisateur appuie dessus.
```cpp
GL::keystrokes.emplace('m', [this]() { manager.get_crashed_aircrafts();});
```

3. Si vous avez fini d'implémenter la gestion du kérosène (Task_2 - Objectif 2 - A), lancez une exception de type `AircraftCrash` lorsqu'un avion tombe à court d'esssence. Normalement, cette exception devrait être traitée de la même manière que lorsqu'un avion s'écrase parce qu'il a attérit trop vite.


**1 && 3**
```cpp
/*dans Aircraft::move()*/
 if (fuel <= 0.0){
                throw AircraftCrash { flight_number + " crashed into the ground because he has used up all his fuel."};
                crashed=true;

            }
/*dans AircraftManager::moov()*/
for (auto& aircraft : aircrafts){
          std::cout << "Aircraft " << aircraft->get_flight_num()<<"  " << (aircraft->has_terminal()?"reserved":"Notreserved") <<"fuel: " << aircraft->get_fuel()  <<std::endl;
        try{
           aircraft->move();
        }catch(const AircraftCrash& err){
             std::cout << err.what() << std::endl;
        }
    }

    auto end = std::remove_if(aircrafts.begin(),  aircrafts.end(),
    [](std::unique_ptr<Aircraft> const &air) {
                                if(air->is_crashed()){
                                    return true;
                                }
                                return air->is_lift() ;
                            });
 
```

4. **BONUS** Rédéfinissez `AircraftCrash` en tant que classe héritant de `std::runtime_error`, plutôt qu'en tant qu'alias. Arrangez-vous pour que son constructeur accepte le numéro de vol, la position, la vitesse de l'avion au moment du crash, et la raison du crash (`"out of fuel"` / `"bad landing"`). Vous utiliserez toutes ces informations pour générer le joli message de l'exception.

## Objectif 2 - Détecter les erreurs de programmation

Pour sécuriser votre code, repassez sur les différentes fonctions de votre programme et ajoutez des assertions permettant de vérifier qu'elles sont correctement utilisées.
Voici quelques idées :
- fonctions d'initialisation appelées une seule fois
- état attendu d'un objet lorsqu'une fonction est appelée dessus
- vérification de certains paramètres de fonctions
- ...

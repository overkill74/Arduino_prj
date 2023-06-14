# Progetto termostato per boliler
Questo progetto riguarda la realizzazione di un termostato per boiler ACS di tipo con resistenza. Si vuole sostituire il termostato elettromeccanico a lamina bimetallica con un moderno sistema di gestione domotico, con possibilità di telegestione. A tale scopo si utilizzerà una scheda Arduino dotata di WiFi.

## Specifiche
La scheda realizzata dovrà sostutire il termostato elettromeccanico del boiler ad ACS. La temperatura della ACS è misurata da un sensore che sostituirà il bulbo di serie dell'apparecchio. Dovrà essere possibile programmare il profilo di temperatura con controllo orario e giornaliero. Deve essere possibile fermare / attivare il controllo di temperatura da remoto, visualizzarne il valore e programmare una doccia. Tutto questo dovrà essere possibile operatdo da remoto. A tale scopo si dovrà realizzare una interfaccia WEB e/o predisporre un soket server, al quale collegarsi con una opportuna APP. Questo potrebbe essere implementato in una fase successiva.
Deve inoltre essere realizzato un acquisitore dati interno che permetta di analizzare l'andamento della temperatura e del consumo, in modo da poter ottimizzare la programmazione oraria del boiler. I dati acquisiti devono coprire il più ampio arco temporale possibile, che deve essere almeno un anno. L'acquisizione dei dati potrebbe anche essere realizzata utilizzando un database esterno.

## Descrizione delle parti
Il progetto è composto dalle seguenti parti:

### Hardware
 1 - Arduino con WiFi
 2 - Sensore di temperatura
 3 - Display (opzionale)
 4 - Relè allo stato solido
 5 - Misuratore di corrente (opzionale)

### Software
 1 - Gestione parametri di programmazione
 2 - Acuisizione dei sensori
 3 - Attuazione dei comandi
 4 - Controllo di temperatura 
 5 - Visualizzazione locale su display
 6 - Webserver per telegestione
 7 - Data acquisition e analytics
 8 - Socket server per gestione remota
## Instalace

    git clone https://github.com/havrlant/cardinality.git
    cd cardinality
    make

Vytvoří se spustitelný soubor `cardinality`. Program bere dva povinné a jeden nepovinný parametr:

    ./cardinality <structure_file> <data_file> [<b>]

- `structure_file` je cesta k CSV souboru se strukturou.
- `data_file` je cesta k CSV souboru s daty.
- `b` je číselný (integer) parametr, který je popsnaný v článku. Lze s jím ovlivnit výpočet kardinality. Výchozí hodnota je 10, očekávané hodnoty jsou v rozmezí 4 až 16. 
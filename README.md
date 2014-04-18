## Instalace

    git clone https://github.com/havrlant/cardinality.git
    cd cardinality
    make

Vytvoří se spustitelný soubor `cardinality`. Program bere dva povinné a jeden nepovinný parametr:

    ./cardinality <structure_file> <data_file> [<b>]

- `structure_file` je cesta k CSV souboru se strukturou.
- `data_file` je cesta k CSV souboru s daty.
- `b` je číselný (integer) parametr, který je popsaný v článku. Parametr ovlivňuje běh algoritmu. Pro větší soubory dat se hodí větší `b`, ale program pak bude zabírat více místa v paměti. Výchozí hodnota je 10, očekávané hodnoty jsou v rozmezí 4 až 16. 
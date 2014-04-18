## Instalace

    git clone https://github.com/havrlant/cardinality.git
    cd cardinality
    make

Vytvoří se spustitelný soubor `cardinality`. Program bere dva povinné a jeden nepovinný parametr:

    ./cardinality <structure_file> <data_file> [<b>]

- `structure_file` je cesta k CSV souboru se strukturou.
- `data_file` je cesta k CSV souboru s daty.
- `b` je číselný (integer) parametr, který je popsaný v článku. Parametr ovlivňuje běh algoritmu. Pro větší soubory dat se hodí větší `b`, ale program pak bude zabírat více místa v paměti. Výchozí hodnota je 10, očekávané hodnoty jsou v rozmezí 4 až 16. 

# Ukázkový výstup

Denní data by se měla spočítat řádově za sekundy, měsíční za minuty. Zabraná paměť so pohybuje v řádů stovek KB, někdy ani to ne. Takto vypadá výstup na mém počítači, pro ilustraci:

    $ time ./cardinality struktura.csv den.csv
    Cely web: 338843
    Sekce c. 1: 165840
    Sekce c. 2: 340716
    Pozice c. 1 (ad_space_pk: 89202): 133324
    Pozice c. 2 (ad_space_pk: 89203): 152526
    Pozice c. 3 (ad_space_pk: 89204): 134439
    Pozice c. 4 (ad_space_pk: 89205): 133324
    Pozice c. 5 (ad_space_pk: 89206): 301022
    Pozice c. 6 (ad_space_pk: 89207): 332565
    Pozice c. 7 (ad_space_pk: 89208): 315814
    Pozice c. 8 (ad_space_pk: 89209): 312487
    Pozice c. 9 (ad_space_pk: 89210): 112661
    Pozice c. 10 (ad_space_pk: 89211): 302439
    Pozice c. 11 (ad_space_pk: 89212): 305222
    Pozice c. 12 (ad_space_pk: 89213): 1433
    Pozice c. 13 (ad_space_pk: 89214): 135193
    Pozice c. 14 (ad_space_pk: 89215): 273999
    Pozice c. 15 (ad_space_pk: 89216): 165337
    Pozice c. 16 (ad_space_pk: 89229): 152526

    real  0m4.684s
    user  0m4.128s
    sys 0m0.141s

    $ time ./cardinality struktura.csv mesic.csv
    Cely web: 4298179
    Sekce c. 1: 1963130
    Sekce c. 2: 4134456
    Pozice c. 1 (ad_space_pk: 89202): 1855783
    Pozice c. 2 (ad_space_pk: 89203): 2042547
    Pozice c. 3 (ad_space_pk: 89204): 1994351
    Pozice c. 4 (ad_space_pk: 89205): 1984089
    Pozice c. 5 (ad_space_pk: 89206): 3833863
    Pozice c. 6 (ad_space_pk: 89207): 4353945
    Pozice c. 7 (ad_space_pk: 89208): 4294845
    Pozice c. 8 (ad_space_pk: 89209): 4294845
    Pozice c. 9 (ad_space_pk: 89210): 1466482
    Pozice c. 10 (ad_space_pk: 89211): 4254127
    Pozice c. 11 (ad_space_pk: 89212): 4242792
    Pozice c. 12 (ad_space_pk: 89213): 24485
    Pozice c. 13 (ad_space_pk: 89214): 1973933
    Pozice c. 14 (ad_space_pk: 89215): 3333132
    Pozice c. 15 (ad_space_pk: 89216): 2039088
    Pozice c. 16 (ad_space_pk: 89229): 2019813

    real  2m23.214s
    user  2m5.840s
    sys 0m4.200s
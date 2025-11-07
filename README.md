
# Pirmoji užduotis – v0.3 konteinerių testavimas

Šioje versijoje programa matuoja std::vector ir std::list konteinerių veikimo spartą.  
Testuojami pagrindiniai etapai:  
- Nuskaitymas iš failo  
- Skirstymas į dvi grupes (vargšiukai ir kietiakiai)  
- Rikiavimas  
- Rašymas į failus

- Pasirenkamai įvykdomas pilnas bench mark testas.

### Naudojimas
>Pirmoji-uzduotis.cpp turi būti nustatyti visi reikalingi studentų failų formatavimo dydžiai(eilučių kiekis).
>Norint sugeneruoti reikalingus .txt failus nustatome *bench_runner.cpp* NEĮTRAUKTI į projektą,
o *Pirmoji-uzduotis.cpp* privalo būti ĮTRAUKTA.
> Paleidus programą galime pasirinkti ar norime naudoti jau sukurtą failą, generuoti naujus arba ranką įrašyti duomenis
> Tada atliekama analizė pagal tolimiau pasirinktus parametrus
BENCH MARK TESTAS
> Norint plaiesti bench mark testą su atsitiktinai sugeneruotais duomenimis *bench_runner.cpp* ĮTRAUKIAME į projektą,
o *Pirmoji-uzduotis.cpp* nustatome NEĮTRAUKTI.
> Tada galime paleisti programą ir su sugeneruotais failais bus vykdomas bench mark testas, kuris nustatys veiksmų su failais laikus.


### Pavyzdinė išvestis
=== Testas su failu: studentai_10000_K6.txt ===
=== v0.3 (vector) ===
Skaitymas: 45 ms
Skaidymas: 20 ms
Rikiavimas: 33 ms
Rasymas: 28 ms
Is viso: 126 ms
Pirmo studento atminties adresas: 0x0000021e96bdfc10

=== v0.3 (list) ===
Skaitymas: 67 ms
Skaidymas: 26 ms
Rikiavimas: 41 ms
Rasymas: 33 ms
Is viso: 167 ms

### Sistemos informacija
- Procesorius: 12th gen Intel i7-12650H
- Operatyvioji atmintis (RAM): 16GB
- Diskas: SSD
- Operacinė sistema:  Windows 11, 64-bit
*Nuadojama Visual Studio 2022*


### Rašomas bendras kompiliavimo laikas su automatiniu benchmark testu
| Įrašų kiekis | Vector bendras (ms) | List bendras (ms) |
|---------------|----------------------|--------------------|
| 1 000         | 63                   | 63                 |
| 10 000        | 672                  | 563                |
| 100 000       | 7781                 | 5692               |
| 1mil          | 127621               | 56693              |
| 10mil         | 199975               | 134431             |               
|

> Visi bėgimai buvo atlikti tame pačiame kompiuteryje, su vienoda apkrova, kai stulpelių kiekis – 6.

 Toliau pateikiami konkretūs rezultatai

 1000 eilučių:

Testuojamas failas: .\studentai_1000_K6.txt 
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (403), kietiakiai.txt (597)
v0.3 (vector) ===
Skaitymas: 33 ms
Skaidymas: 2 ms
Rikiavimas: 13 ms
Rasymas: 15 ms
Is viso: 63 ms
Pirmo studento atminties adresas: 0000016188E54080
---------------------------------------
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (403), kietiakiai.txt (597)
v0.3 (list) ===
Skaitymas: 43 ms
Skaidymas: 2 ms
Rikiavimas: 0 ms
Rasymas: 18 ms
Is viso: 63 ms
Pirmo studento atminties adresas: 00000161DEC024B0
---------------------------------------

10000 eilučių:

=== Testuojamas failas: .\studentai_10000_K6.txt ===
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (4050), kietiakiai.txt (5950)
v0.3 (vector) ===
Skaitymas: 315 ms
Skaidymas: 21 ms
Rikiavimas: 212 ms
Rasymas: 124 ms
Is viso: 672 ms
Pirmo studento atminties adresas: 0000016188E5B080
---------------------------------------
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (4050), kietiakiai.txt (5950)
v0.3 (list) ===
Skaitymas: 392 ms
Skaidymas: 22 ms
Rikiavimas: 10 ms
Rasymas: 139 ms
Is viso: 563 ms
Pirmo studento atminties adresas: 00000161D982EC00
---------------------------------------

100000 eilučių

=== Testuojamas failas: .\studentai_100000_K6.txt ===
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (41364), kietiakiai.txt (58636)
v0.3 (vector) ===
Skaitymas: 3750 ms
Skaidymas: 199 ms
Rikiavimas: 2675 ms
Rasymas: 1157 ms
Is viso: 7781 ms
Pirmo studento atminties adresas: 00000161E0716080
---------------------------------------
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (41364), kietiakiai.txt (58636)
v0.3 (list) ===
Skaitymas: 4011 ms
Skaidymas: 230 ms
Rikiavimas: 133 ms
Rasymas: 1318 ms
Is viso: 5692 ms
Pirmo studento atminties adresas: 00000161DD989E70
---------------------------------------

1mil eilučių:

=== Testuojamas failas: .\studentai_1000000_K6.txt ===
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (410504), kietiakiai.txt (589496)
v0.3 (vector) ===
Skaitymas: 44709 ms
Skaidymas: 2672 ms
Rikiavimas: 66212 ms
Rasymas: 14028 ms
Is viso: 127621 ms
Pirmo studento atminties adresas: 000001623821B080
---------------------------------------
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (410504), kietiakiai.txt (589496)
v0.3 (list) ===
Skaitymas: 39450 ms
Skaidymas: 2324 ms
Rikiavimas: 1672 ms
Rasymas: 13193 ms
Is viso: 56639 ms
Pirmo studento atminties adresas: 00000161F87524B0
---------------------------------------

10mil eilučių:

=== Testuojamas failas: .\studentai_10000000_K6.txt ===
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (916677), kietiakiai.txt (1314309)
v0.3 (vector) ===
Skaitymas: 81654 ms
Skaidymas: 4306 ms
Rikiavimas: 83059 ms
Rasymas: 30956 ms
Is viso: 199975 ms
Pirmo studento atminties adresas: 00000161A8FFB080
---------------------------------------
Rezultatai issaugoti faile: vargsiukai.txt
Rezultatai issaugoti faile: kietiakiai.txt
Rezultatai issaugoti: vargsiukai.txt (916677), kietiakiai.txt (1314309)
v0.3 (list) ===
Skaitymas: 109163 ms
Skaidymas: 3319 ms
Rikiavimas: 2555 ms
Rasymas: 19394 ms
Is viso: 134431 ms
Pirmo studento atminties adresas: 00000161D6E6A180
---------------------------------------

## Skaidymo strategijos (v1.0)

### Strategija 1 - `partition_copy`
Naudojami du nauji konteineriai: vienas vargšiukams, kitas kietiakiams.  
Naudojamas algoritmas `std::partition_copy`, kuris per vieną perėjimą per duomenis sukuria abi grupes.  
*Privalumai: greitas skaidymas, paprastas realizavimas.  
*Trūkumai: dvigubas atminties naudojimas.

### Strategija 2 – `remove_if` + `copy_if`
Vargšiukai nukopijuojami į naują konteinerį, o iš pradinio pašalinami naudojant `std::remove_if`.  
*Privalumai: mažesnis atminties poreikis, paprasta kontrolė.  
*Trūkumai: dvigubas perėjimas per duomenis, lėtesnis dideliems failams.

### Strategija 3 - `partition` (in-place)
Skirstymas vykdomas vietoje, viename konteineryje naudojant `std::partition`.  
*Privalumai: mažiausias atminties naudojimas, labai greitas.  
*Trūkumai: gali pakeisti studentų eiliškumą.

---

### Testavimo planas
Bus atliktas visų trijų strategijų palyginimas, matuojant **skaidymo laiką** (ms) ir **atminties naudojimą** skirtingiems duomenų kiekiams:  
| Įrašų kiekis | Strategija 1 | Strategija 2 | Strategija 3 |
|---------------|---------------|---------------|---------------|
| 1 000         | 
| 10 000        | 
| 100 000       | 
| 1mil          |

*10 mil. kol kas nerasome, del laiko taupymo*




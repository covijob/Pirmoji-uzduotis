
# Pirmoji užduotis – v0.3 konteinerių testavimas

Šioje versijoje programa matuoja **std::vector** ir **std::list** konteinerių veikimo spartą.  
Testuojami pagrindiniai etapai:  
- Nuskaitymas iš failo  
- Skirstymas į dvi grupes (vargšiukai ir kietiakiai)  
- Rikiavimas  
- Rašymas į failus  

### Naudojimas
1. Paleisti programą ir pasirinkti **duomenų šaltinį (2 – generavimas)**.  
2. Programa sugeneruos kelis failus su skirtingu įrašų kiekiu (1000, 10000, 100000).  
3. Kiekvienam sugeneruotam failui bus automatiškai paleisti testai su **vector** ir **list** konteineriais.  
4. Kiekvieno testo pabaigoje išvedami etapai (skaitymas, skirstymas, rikiavimas, rašymas) bei bendras laikas.

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

### Vidurkių rezultatai (3 bėgimai)

Kiekvienas testas buvo vykdomas 3 kartus, o lentelėje pateikiami vidutiniai laikai milisekundėmis (ms):

| Įrašų kiekis | Vector vidurkis (ms) | List vidurkis (ms) |
|---------------|----------------------|--------------------|
| 1 000         | —                    | —                  |
| 10 000        | —                    | —                  |
| 100 000       | —                    | —                  |
| 1mil          | –
| 10mil         | –               
|


> Pastaba: lentelė bus užpildyta atlikus faktinius testus.  
> Visi bėgimai buvo atlikti tame pačiame kompiuteryje, su vienoda apkrova.

### Sistemos informacija
- **Procesorius: 12th gen Intel i7-12650H
- **Operatyvioji atmintis (RAM): 16GB
- **Diskas: SSD
- **Operacinė sistema:**  Windows 11, 64-bit



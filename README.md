
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
## Tabel A1: effect van murendichtheid op onderzochte cellen

*Vast: 500 x 500, max kost 10, n = 100 doolhoven per cel. Gemiddelden.*

| Dichtheid | BFS | DFS | Dijkstra | A* |
|---|---|---|---|---|
| 0% | 126.689 | 113.254 | 124.259 | 90.204 |
| 10% | 126.735 | 124.505 | 124.460 | 91.546 |
| 20% | 126.664 | 110.303 | 124.746 | 92.989 |
| 30% | 126.391 | 111.941 | 124.807 | 94.473 |
| 40% | 126.077 | 112.796 | 124.080 | 95.271 |
| 50% | 125.572 | 117.586 | 124.075 | 96.928 |
| 60% | 125.015 | 116.778 | 124.266 | 98.836 |
| 70% | 124.196 | 123.627 | 123.731 | 100.582 |
| 80% | 123.551 | 123.813 | 123.615 | 103.405 |
| 90% | 122.670 | 121.971 | 122.932 | 107.088 |
| 100% | 129.774 | 125.138 | 129.786 | 129.586 |

## Tabel A2: effect van murendichtheid op maximale frontier

*Vast: 500 x 500, max kost 10, n = 100 doolhoven per cel. Gemiddelden.*

| Dichtheid | BFS | DFS | Dijkstra | A* |
|---|---|---|---|---|
| 0% | 526,6 | 113.203 | 932,9 | 949,2 |
| 10% | 533,8 | 119.556 | 900,5 | 910,5 |
| 20% | 540,4 | 102.629 | 868,6 | 874,2 |
| 30% | 545,3 | 99.452 | 832,5 | 831,5 |
| 40% | 548,3 | 95.206 | 791,2 | 788,0 |
| 50% | 549,3 | 92.401 | 746,8 | 737,9 |
| 60% | 544,3 | 84.473 | 698,3 | 687,0 |
| 70% | 530,9 | 78.959 | 639,9 | 623,7 |
| 80% | 499,8 | 65.873 | 561,5 | 552,0 |
| 90% | 423,2 | 46.055 | 448,6 | 434,4 |
| 100% | 26,5 | 47.080 | 27,1 | 26,7 |

## Tabel B: effect van grootte op onderzochte cellen

*Vast: murendichtheid 50%, max kost 10. Aandeel = onderzochte cellen / aantal cellen.*

| Grootte | Cellen | BFS | DFS | Dijkstra | A* | Aandeel BFS | Aandeel DFS | Aandeel Dijkstra | Aandeel A* |
|---|---|---|---|---|---|---|---|---|---|
| 10 x 10 | 100,0 | 50,5 | 55,6 | 50,0 | 41,0 | 50,5% | 55,6% | 50,0% | 41,0% |
| 20 x 20 | 400,0 | 189,3 | 197,4 | 186,0 | 147,0 | 47,3% | 49,3% | 46,5% | 36,8% |
| 50 x 50 | 2.500 | 1.099 | 1.202 | 1.104 | 853,9 | 44,0% | 48,1% | 44,2% | 34,2% |
| 100 x 100 | 10.000 | 5.271 | 5.156 | 5.240 | 4.085 | 52,7% | 51,6% | 52,4% | 40,8% |
| 200 x 200 | 40.000 | 19.255 | 18.079 | 19.187 | 14.668 | 48,1% | 45,2% | 48,0% | 36,7% |
| 500 x 500 | 250.000 | 125.572 | 117.586 | 124.075 | 96.928 | 50,2% | 47,0% | 49,6% | 38,8% |
| 1000 x 1000 | 1.000.000 | 513.157 | 537.392 | 510.663 | 390.101 | 51,3% | 53,7% | 51,1% | 39,0% |
| 10000 x 10000 | 100.000.000 | 45.023.177 | 50.830.658 | 44.631.373 | 35.613.754 | 45,0% | 50,8% | 44,6% | 35,6% |

## Tabel C: geheugengebruik

*Bytes, gemiddeld over alle dichtheden en kosten. Meet wat de implementaties aanvragen (zonder het doolhof zelf), niet wat de algoritmes in principe nodig hebben.*

| Grootte | Totaal BFS | Totaal DFS | Totaal Dijkstra | Totaal A* | Piek BFS | Piek DFS | Piek Dijkstra | Piek A* | Piek/cel BFS | Piek/cel DFS | Piek/cel Dijkstra | Piek/cel A* |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 10 x 10 | 1.256 | 1.256 | 9.164 | 9.164 | 1.256 | 1.256 | 9.164 | 9.164 | 12,6 | 12,6 | 91,6 | 91,6 |
| 20 x 20 | 4.856 | 4.856 | 11.864 | 11.864 | 4.856 | 4.856 | 11.864 | 11.864 | 12,1 | 12,1 | 29,7 | 29,7 |
| 50 x 50 | 30.056 | 30.056 | 30.764 | 30.764 | 30.056 | 30.056 | 30.764 | 30.764 | 12,0 | 12,0 | 12,3 | 12,3 |
| 100 x 100 | 120.056 | 120.056 | 98.264 | 98.269 | 120.056 | 120.056 | 98.264 | 98.269 | 12,0 | 12,0 | 9,8 | 9,8 |
| 200 x 200 | 480.056 | 480.056 | 368.264 | 368.582 | 480.056 | 480.056 | 368.264 | 368.567 | 12,0 | 12,0 | 9,2 | 9,2 |
| 500 x 500 | 3.000.056 | 3.000.056 | 2.259.371 | 2.266.938 | 3.000.056 | 3.000.056 | 2.259.371 | 2.265.736 | 12,0 | 12,0 | 9,0 | 9,1 |
| 1000 x 1000 | 12.000.056 | 12.000.056 | 9.023.347 | 9.048.598 | 12.000.056 | 12.000.056 | 9.022.548 | 9.041.449 | 12,0 | 12,0 | 9,0 | 9,0 |
| 10000 x 10000 | 1.200.000.056 | 1.200.000.056 | 900.243.002 | 902.382.355 | 1.200.000.056 | 1.200.000.056 | 900.188.041 | 901.792.556 | 12,0 | 12,0 | 9,0 | 9,0 |

## Tabel D: uitvoeringstijd in ms (gemiddelde +/- standaardafwijking)

*Vast: murendichtheid 50%, max kost 10. Alleen vanaf 500 x 500: de klok meet in hele milliseconden.*

| Grootte | n | BFS | DFS | Dijkstra | A* |
|---|---|---|---|---|---|
| 500 x 500 | 100 | 12,5 +/- 7,16 | 15,7 +/- 11,6 | 40,0 +/- 23,1 | 35,7 +/- 23,8 |
| 1000 x 1000 | 50 | 53,3 +/- 26,0 | 76,5 +/- 53,2 | 178,2 +/- 90,6 | 152,6 +/- 92,0 |
| 10000 x 10000 | 10 | 5.356 +/- 3.647 | 7.034 +/- 4.777 | 20.766 +/- 14.389 | 18.564 +/- 14.723 |

## Tabel E: kwaliteit van het pad

*500 x 500. 1,00 = optimale kost (Dijkstra). A* heeft dezelfde kost als Dijkstra, dus A* staat niet in de verhoudingskolommen.*

| Situatie | Lengte BFS | Lengte DFS | Lengte Dijkstra | Lengte A* | Kost BFS | Kost DFS | Kost Dijkstra | Kost A* | Kost BFS / Dijkstra | Kost DFS / Dijkstra |
|---|---|---|---|---|---|---|---|---|---|---|
| 0%, kost 1 | 346,0 | 113.202 | 346,0 | 346,0 | 346,0 | 113.202 | 346,0 | 346,0 | 1,00 | 327,19 |
| 0%, kost 10 | 346,0 | 113.202 | 361,8 | 362,3 | 1.893 | 622.467 | 1.150 | 1.150 | 1,65 | 541,47 |
| 50%, kost 1 | 353,9 | 79.181 | 353,9 | 353,9 | 353,9 | 79.181 | 353,9 | 353,9 | 1,00 | 223,73 |
| 50%, kost 10 | 353,9 | 79.181 | 378,0 | 378,3 | 1.943 | 435.472 | 1.468 | 1.468 | 1,32 | 296,62 |
| 100%, kost 1 | 38.086 | 38.086 | 38.086 | 38.086 | 38.086 | 38.086 | 38.086 | 38.086 | 1,00 | 1,00 |
| 100%, kost 10 | 38.086 | 38.086 | 38.086 | 38.086 | 209.457 | 209.457 | 209.457 | 209.457 | 1,00 | 1,00 |

## Tabel F1: A* onderzochte cellen als aandeel van Dijkstra (500 x 500)

*500 x 500, per murendichtheid, n = 100. Onder 100% onderzoekt A* minder dan Dijkstra.*

| Max kost | 0% | 10% | 20% | 30% | 40% | 50% | 60% | 70% | 80% | 90% | 100% |
|---|---|---|---|---|---|---|---|---|---|---|---|
| kost 1 | 17,3% | 16,4% | 15,4% | 14,2% | 12,8% | 11,5% | 11,7% | 14,8% | 22,9% | 37,8% | 99,2% |
| kost 5 | 54,9% | 56,1% | 57,5% | 59,1% | 60,7% | 62,7% | 64,9% | 67,5% | 71,1% | 77,0% | 99,7% |
| kost 10 | 72,6% | 73,6% | 74,5% | 75,7% | 76,8% | 78,1% | 79,5% | 81,3% | 83,7% | 87,1% | 99,8% |

## Tabel F2: A* onderzochte cellen als aandeel van Dijkstra (10000 x 10000)

*10000 x 10000, per murendichtheid, n = 10. Onder 100% onderzoekt A* minder dan Dijkstra.*

| Max kost | 0% | 10% | 20% | 30% | 40% | 50% | 60% | 70% | 80% | 90% | 100% |
|---|---|---|---|---|---|---|---|---|---|---|---|
| kost 1 | 14,7% | 13,6% | 12,2% | 11,2% | 10,5% | 9,6% | 8,9% | 12,1% | 19,3% | 33,4% | 99,9% |
| kost 5 | 52,9% | 54,5% | 56,2% | 57,9% | 59,9% | 62,2% | 65,0% | 68,3% | 72,5% | 78,8% | 100,0% |
| kost 10 | 73,8% | 74,9% | 76,0% | 77,1% | 78,4% | 79,8% | 81,4% | 83,2% | 85,3% | 88,5% | 100,0% |

## Tabel G: tijd tegenover onderzochte cellen

*10000 x 10000, dichtheid 50%, n = 10.*

| Max kost | Algoritme | Onderzochte cellen | Tijd (ms) | Tijd per cel (microseconden) |
|---|---|---|---|---|
| 1 | BFS | 45.023.177 | 5.478 | 0,122 |
| 1 | DFS | 50.830.658 | 6.991 | 0,138 |
| 1 | Dijkstra | 45.023.744 | 18.966 | 0,421 |
| 1 | A* | 4.323.014 | 1.998 | 0,462 |
| 5 | BFS | 45.023.177 | 5.441 | 0,121 |
| 5 | DFS | 50.830.658 | 7.009 | 0,138 |
| 5 | Dijkstra | 44.584.040 | 20.632 | 0,463 |
| 5 | A* | 27.731.166 | 14.557 | 0,525 |
| 10 | BFS | 45.023.177 | 5.356 | 0,119 |
| 10 | DFS | 50.830.658 | 7.034 | 0,138 |
| 10 | Dijkstra | 44.631.373 | 20.766 | 0,465 |
| 10 | A* | 35.613.754 | 18.564 | 0,521 |

## Tabel H: beste algoritme per meetwaarde en situatie

*500 x 500. Laagste waarde wint; verschil onder 1% telt als gelijkspel. Geheugen hangt niet van dichtheid of kost af. Controleer ook met 200 en 1000.*

| Meetwaarde | 0%, kost 1 | 0%, kost 10 | 50%, kost 1 | 50%, kost 10 | 100%, kost 1 | 100%, kost 10 |
|---|---|---|---|---|---|---|
| Padlengte | BFS / Dijkstra / A* | BFS | BFS / Dijkstra / A* | BFS | BFS / DFS / Dijkstra / A* | BFS / DFS / Dijkstra / A* |
| Padkost | BFS / Dijkstra / A* | Dijkstra / A* | BFS / Dijkstra / A* | Dijkstra / A* | BFS / DFS / Dijkstra / A* | BFS / DFS / Dijkstra / A* |
| Onderzochte cellen | A* | A* | A* | A* | DFS | DFS |
| Frontier | BFS / Dijkstra | BFS | BFS / Dijkstra | BFS | BFS / Dijkstra | BFS / A* |
| Tijd | A* | DFS | A* | BFS | BFS | BFS |
| Geheugen (piek) | Dijkstra / A* | Dijkstra / A* | Dijkstra / A* | Dijkstra / A* | Dijkstra / A* | Dijkstra / A* |

## Tabel I: gevoeligheid per factor (grootste waarde / kleinste waarde)

*Eén factor loopt over het hele bereik, de rest op 500 x 500, 50%, kost 10. Alles gedeeld door het aantal cellen. Tijd voor grootte: vanaf 500. 1,00 = geen invloed.*

| Meetwaarde (per cel) | Algoritme | Grootte | Murendichtheid | Max kost |
|---|---|---|---|---|
| cellen/cel | BFS | 1,20 | 1,06 | 1,00 |
| cellen/cel | DFS | 1,23 | 1,13 | 1,00 |
| cellen/cel | Dijkstra | 1,19 | 1,06 | 1,01 |
| cellen/cel | A* | 1,20 | 1,44 | 6,69 |
| frontier/cel | BFS | 1.154,30 | 20,73 | 1,00 |
| frontier/cel | DFS | 1,28 | 2,60 | 1,00 |
| frontier/cel | Dijkstra | 974,81 | 34,39 | 1,36 |
| frontier/cel | A* | 909,17 | 35,59 | 3,18 |
| padkost/cel | BFS | 1.250,17 | 110,64 | 5,49 |
| padkost/cel | DFS | 1,29 | 3,30 | 5,50 |
| padkost/cel | Dijkstra | 1.518,93 | 182,20 | 4,15 |
| padkost/cel | A* | 1.518,93 | 182,20 | 4,15 |
| tijd/cel | BFS | 1,07 | 1,23 | 1,00 |
| tijd/cel | DFS | 1,22 | 2,04 | 1,01 |
| tijd/cel | Dijkstra | 1,30 | 1,94 | 1,12 |
| tijd/cel | A* | 1,30 | 1,43 | 6,77 |

## Tabel J: invloed van elke factor op de verhouding tussen algoritmes

*Eén factor loopt over het bereik, de rest op 500 x 500, 50%, kost 10. Elke cel: kleinste tot grootste verhouding. Hoe breder het bereik, hoe meer de factor het verschil bepaalt.*

| Factor | A* / Dijkstra (cellen) | BFS / Dijkstra (padkost) | DFS / BFS (padlengte) |
|---|---|---|---|
| Grootte | 0,76 tot 0,82 | 1,11 tot 1,35 | 5,16 tot 5.875,81 |
| Murendichtheid | 0,73 tot 1,00 | 1,00 tot 1,65 | 1,00 tot 327,19 |
| Max kost | 0,12 tot 0,78 | 1,00 tot 1,32 | 223,73 tot 223,73 |

## Tabel K: doolhofstructuur en efficientie

*Kost 10. Inspanning = onderzochte cellen / cellen. Kwaliteit = padkost / padkost Dijkstra (1,00 = optimaal).*

| Structuur | Inspanning BFS | Inspanning DFS | Inspanning Dijkstra | Inspanning A* | Kwaliteit BFS | Kwaliteit DFS | Kwaliteit Dijkstra | Kwaliteit A* |
|---|---|---|---|---|---|---|---|---|
| 10 x 10, 0% | 49,5% | 54,6% | 49,0% | 37,0% | 1,31 | 10,68 | 1,00 | 1,00 |
| 10 x 10, 50% | 50,5% | 55,6% | 50,0% | 41,0% | 1,11 | 5,85 | 1,00 | 1,00 |
| 10 x 10, 100% | 50,5% | 48,7% | 50,6% | 49,8% | 1,00 | 1,00 | 1,00 | 1,00 |
| 100 x 100, 0% | 52,8% | 50,1% | 52,9% | 37,9% | 1,56 | 113,04 | 1,00 | 1,00 |
| 100 x 100, 50% | 52,7% | 51,6% | 52,4% | 40,8% | 1,27 | 60,45 | 1,00 | 1,00 |
| 100 x 100, 100% | 45,3% | 48,6% | 45,3% | 45,2% | 1,00 | 1,00 | 1,00 | 1,00 |
| 500 x 500, 0% | 50,7% | 45,3% | 49,7% | 36,1% | 1,65 | 541,47 | 1,00 | 1,00 |
| 500 x 500, 50% | 50,2% | 47,0% | 49,6% | 38,8% | 1,32 | 296,62 | 1,00 | 1,00 |
| 500 x 500, 100% | 51,9% | 50,1% | 51,9% | 51,8% | 1,00 | 1,00 | 1,00 | 1,00 |
| 1000 x 1000, 0% | 51,8% | 44,1% | 51,1% | 35,8% | 1,66 | 1.084,73 | 1,00 | 1,00 |
| 1000 x 1000, 50% | 51,3% | 53,7% | 51,1% | 39,0% | 1,33 | 611,06 | 1,00 | 1,00 |
| 1000 x 1000, 100% | 53,5% | 47,0% | 53,5% | 53,4% | 1,00 | 1,00 | 1,00 | 1,00 |
| 10000 x 10000, 0% | 45,6% | 62,2% | 44,7% | 33,0% | 1,68 | 17.169,04 | 1,00 | 1,00 |
| 10000 x 10000, 50% | 45,0% | 50,8% | 44,6% | 35,6% | 1,35 | 7.918,95 | 1,00 | 1,00 |
| 10000 x 10000, 100% | 58,6% | 55,7% | 58,6% | 58,6% | 1,00 | 1,00 | 1,00 | 1,00 |


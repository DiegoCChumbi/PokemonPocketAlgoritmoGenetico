# Deckbuilding consistente con algoritmos genéticos

## Contexto
Se tomará en cuenta las 226 cartas de la primera expansión del Pokemon JJC Pocket, Genetic Apex.
y las 7 primera cartas del sobre Promo-A, 

Genetic Apex:
- 215 Pokemon
- 3 Items
- 8 Partidarios
Promo-A
- 6 Items
- 1 Partidario

Total: 234 cartas

Cada deck esta compuesto por 20 cartas, con un máximo de 2 copias por carta. Con mínimo un pokemon básico.

## Tipo de problema
Optimización combinatoria en su variantes de "portfolio problem", cada carta para sacarle provecho requiere de otras cartas especificas, además hay limite de cartas por deck y indicadores de consistencia que se buscaría maximizar.

## Definición de consistente

- Promedio de cantidad de acciones por turno disponibles con una mano de 5 cartas al azar.
- Sinergia entre cartas
- Cantidad de tipos de energías (<=2)
- Mínima cantidad de líneas evolutivas inconclusas. , cada carta para sacarle provecho requiere de otras cartas especificas, además hay limite de cartas por deck y indicadores de consistencia que se buscaría maximizar.

## Para ejecutar

```
./compile_run.bat
```
Compilará el programa y generará un archivo de texto con la salida.

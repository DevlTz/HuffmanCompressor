# Design: formato do arquivo comprimido (.huf) e representação da árvore

## Header
- 4 bytes: ASCII 'H','U','F','\0'
- 1 byte: versão (ex: 1)
- 2 bytes: flags/reservado

## Serialização da árvore (pré-order)
Para cada nó na pré-order:
- se nó interno: escrever um bit 0
- se folha: escrever um bit 1 seguido do símbolo (1 byte)

Isso produz uma representação com 1 bit por nó interno/folha + 8 bits para cada folha (símbolo).
Armazene o tamanho desta serialização antes dos bytes (4 ou 8 bytes) para leitura.

## Payload
Concatenar códigos (bits) de cada símbolo da entrada; agrupar em bytes, adicionando padding no final.
No final do arquivo, escrever 1 byte com a quantidade de bits válidos no último byte (0..7) ou incluir isso no header.

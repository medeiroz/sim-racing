# Sim racing

Trabalho em progresso

## O que é?

Sim racing é um tipo de competição de automobilismo virtual, onde os pilotos competem em corridas de carros de corrida em simuladores de corrida de computador. O sim racing é uma forma de simulação de corrida de automóveis que é usada para treinar pilotos de corrida, para fornecer uma experiência de corrida realista e para fornecer entretenimento para os jogadores de simulação de corrida.

## Objetivo do repositório

Este projeto tem como o objetivo a criação de um simulador de corrida de carros.

## Referencias

- [FFBeast](https://ffbeast.github.io/docs/en/wheel.html)
- [A Chave Virou](https://www.youtube.com/watch?v=a1Q18tryYvI)
  - Excelente video sobre a construção do volante + force feedback

---

## Volante + force feedback

### Pra que serve o volante?

Controla a direção do carro e simula a força que o piloto sente ao dirigir um carro de corrida.

Todas as sensações que o piloto sente ao dirigir um carro de corrida são transmitidas para o volante, como:

- Curvas
- Aceleração
- Frenagem
- Piso molhado
- Piso seco
- Piso irregular
- Zebras
- Colisões

### Bibliotecas utilizadas no volante

- [FFBeast](https://ffbeast.github.io/)
  - Utilizado para a criação do force feedback

### Funcionalidades do volante

- Force feedback
- Centralização do volante
- Indicador de volante ligado

### Modelos 3D do volante

- [Case estilo fanatec](https://cults3d.com/en/3d-model/game/fanatec-style-case-for-hoverboard-sim-wheel)
  - Utilizado para acomodar o motor de hoverboard
- [Prensa de mesa](https://cults3d.com/en/3d-model/game/table-clamp-for-fanatec)
  - Utilizado para preender a case na mesa

### Hardware e peças do volante

- Motor de hoverboard
  - Utilizado para o force feedback
- [ODESC 4.2](https://pt.aliexpress.com/item/1005005763345874.html)
  - Utilizado para controlar o motor de hoverboard junto ao FFBEAST
- [ENDER 3 v3 KE](https://www.creality.com/products/creality-ender-3-v3-ke)
  - Impressora 3D utilizado para a impressão 3D de peças
- [1x Botão momentaneo](https://pt.aliexpress.com/item/1005005904666959.html)
  - Botão utilizado para centralizar o volante
- [Resistor de frenagem 50w 10ohm](https://pt.aliexpress.com/item/1005006825516125.html)
- [Fonte 12v 30a](https://produto.mercadolivre.com.br/MLB-3732855248-fonte-24v-30a-720w-chaveada-estabilizada-bivolt-cabo-_JM)
- [Encoder Rotativo 1000ppr](https://pt.aliexpress.com/item/1005005779039462.html)
- [2x Cooler 24v](https://pt.aliexpress.com/item/1005002468157849.html)

### Esquema eletrico do volante

@todo

---

## Aro do volante

### Pra que serve o aro?

O aro do volante é a parte do volante que o piloto segura para controlar o carro.

Tem alguns botoes para controlar o carro e o jogo.

Utilize a programação da pasta `volante_botoes` para a programação do ESP32.

### Bibliotecas utilizadas no aro

- [ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad/)
  - Biblioteca utilizada no ESP32 para transformar a placa em um controle de jogos via bluetooth
- [Bounce2](https://github.com/thomasfredericks/Bounce2)
  - Biblioteca utilizada para a leitura de botões com debounce evitando multiplos acionamentos
- [TickTwo](https://github.com/sstaub/TickTwo)
  - Biblioteca utilizada para a criação de timers
  - Utilizada para a criação de um timer para controlar a `piscada` do led On/Off e de conexão

### Funcionalidades do aro

- Ações dentro do jogo
  - Troca de marcha
  - Comunicação com a equipe
  - Controle de tração
  - Controle de freio
  - Pause / Resume
  - Menu
- On/Off
- Conexão via bluetooth ou cabo USB C
- 12 botões personalizaveis
- 2 trocas de marcha
- Indicador de volante ligado
- Indicador de volante conectado ao computador

### Modelos 3D do aro

- [Aro do volante F1](https://www.thingiverse.com/thing:4891321)
  - Utilizado para a impressão do aro do volante
- [Saque rapido / Quick Release](https://www.thingiverse.com/thing:2749233)
  - Utilizado para trocar o aro do volante rapida e facilmente

### Hardware e peças do aro

- [ESP32 devkit](https://pt.aliexpress.com/item/1005004605399313.html)
  - Utilizado para a comunicação entre o volante e o computador
- [ENDER 3 v3 KE](https://www.creality.com/products/creality-ender-3-v3-ke)
- [12x Botão momentaneo](https://pt.aliexpress.com/item/1005005904666959.html)
  - Botões utilizados para os controles do volante
- [2x micro switch](https://pt.aliexpress.com/item/1005006085607858.html)
  - Micro switch utilizados para as trocas de marcha
- [2x bateria 18650](https://pt.aliexpress.com/item/1005005638404503.html)
  - Baterias utilizadas para alimentar o volante
- [Placa de carregamento TP4056](https://pt.aliexpress.com/item/1005004427739715.html)
  - Placa utilizada para o carregamento das baterias
- [kit conector UBC type C macho + femea](https://pt.aliexpress.com/item/1005007458786256.html)
  - Conectores utilizados para ligação entre carcaça -> esp32
- [LED 3mm verde](https://pt.aliexpress.com/item/1005003320296052.html)
  - LED utilizado para indicar se o volante esta ligado
  - O mesmo led é utilizado para indicar se o volante esta conectado ao computador
- [Resistor 150ohm](https://pt.aliexpress.com/item/1005001627995396.html)
  - Resistor utilizado para limitar a corrente do led
- [Chave liga/desliga](https://produto.mercadolivre.com.br/MLB-1591116434-10x-mini-chave-gangorra-kcd11-101-ligadesliga-2t-3a-250v-pt-_JM)
  - Chave utilizada para ligar/desligar o volante

### Esquema eletrico do aro

@todo

---

## Pedais

### Pra que serve os pedais?

Os pedais são utilizados para controlar a aceleração, frenagem e embreagem do carro.

Por enquanto só temos o acelerador e o freio. Não teremos embreagem pois utilizaremos o cambio via borboleta no volante.

Utilize a programação da pasta `pedals_joystick` para a programação do Arduino Pro Micro.

### Bibliotecas utilizadas nos pedais

- [SimRacing](https://github.com/dmadison/Sim-Racing-Arduino)
  - Biblioteca utilizada para a comunicação entre os pedais e o computador
- [Joystick](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  - É uma dependencia da biblioteca `SimRacing`
- [Bounce2](https://github.com/thomasfredericks/Bounce2)
  - Biblioteca utilizada para a leitura de botões com debounce evitando multiplos acionamentos

### Funcionalidades dos pedais

- Acelerador
- Freio
- Calibração dos pedais
  - Botão momentaneo para calibrar os pedais
  - Calibração do minimo e maximo dos pedais pisando os pedais no minimo e maximo
- Indicador de calibração
  - LED off -> Pedal não calibrado e calibração não iniciada
  - LED on -> Pedal calibrado
  - LED piscando -> Pedal não calibrado e calibração iniciada

### Modelos 3D dos pedais

- [SIM RACING PEDAL](https://cults3d.com/en/3d-model/game/sim-racing-pedal)
  - Utilizado para a impressão dos pedais
  - Acelerador
  - Freio

### Hardware e peças dos pedais

- [Arduino Pro Micro Type C](https://pt.aliexpress.com/item/1005006654641959.html)
  - Controladora dos pedais e comunicação do hardware com o computador
- [ENDER 3 v3 KE](https://www.creality.com/products/creality-ender-3-v3-ke)
- [Botao momentaneo](https://pt.aliexpress.com/item/1005005904666959.html)
  - Botão utilizado para calibrar o minimo e o maximo dos pedais
- [2x Molas de rodoar de caminhao](https://produto.mercadolivre.com.br/MLB-1501615041-mola-cromada-rodoar-rodo-ar-caminhao-15-unidades-_JM)
  - Molas utilizadas para o retorno dos pedais a posição inicial
- [2x Sensor hall 49e](https://pt.aliexpress.com/item/32852539663.html)
  - Sensor utilizado para a leitura das posições dos pedais
- [2x Imãs de neodimino 8x5mm](https://produto.mercadolivre.com.br/MLB-1463589800-im-neodimio-8x5-mm-n35-10-unidades-_JM)
- [10x Rolamento 608Zz](https://www.mercadolivre.com.br/rolamentos-608-zz-abec-1-kit-com-20-pcs-8x22x7/p/MLB35811125)
  - Rolamentos utilizados para as partes moveis dos pedais
  - Utilizado para evitar o desgaste das peças
- [2x Capacitor 10uf](https://pt.aliexpress.com/item/32607817053.html)
  - Capacitor utilizado junto ao resistor de 220ohm para criar um filtro passa baixa para o sensor hall
    - Serve para eliminar os ruidos do sensor hall
- [3x resistor 220ohm](https://pt.aliexpress.com/item/1005001627995396.html)
  - Resistor utilizado junto ao capacitor 10uf para criar um filtro passa baixa para o sensor hall
    - Serve para eliminar os ruidos do sensor hall
  - Resistor utilizado para limitar a corrente do led
- [Led 3mm verde](https://pt.aliexpress.com/item/1005003320296052.html)
  - LED utilizado para indicar se o pedal esta calibrado ou precisa de calibração
- [2x Parafuso de cama 120mm](https://www.mercadolivre.com.br/parafuso-para-cama-com-porca-516-x-120mm-12-unid/p/MLB29495512)
  - Parafusos utilizados como eixo e cursor dos pedais

### Esquema eletrico dos pedais

@todo

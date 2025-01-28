# Sim racing

## 🚧 Trabalho em Progresso

Este repositório documenta o desenvolvimento de um volante e pedais para simulação de corrida (*sim racing*) com funcionalidades avançadas, incluindo force feedback, comunicação via Bluetooth, e diversos componentes personalizados. Cada seção detalha os passos, componentes e funcionalidades integradas ao projeto.

---

## Índice

1. [Roadmap](#roadmap)
2. [O que é?](#o-que-é)
3. [Objetivo do Repositório](#objetivo-do-repositório)
4. [Referências](#referencias)
5. [Volante + Force Feedback](#volante--force-feedback)
6. [Aro do Volante](#aro-do-volante)
7. [Pedais](#pedais)
8. [Contribuição](#contribuição)
9. [Licença](#licença)

---

## Roadmap

- [x] Volante com force feedback
  - [x] Impressão 3D do case
  - [x] Compra das peças
  - [x] Montagem do volante
    - [x] Adicionar botão de centralização
    - [x] Adicionar cooler para resfriamento
    - [x] Trocar o conector de eneria para um mais robusto
  - [x] Programação do force feedback
  - [x] Testes e ajustes
- [x] Aro do volante
  - [x] Impressão 3D do aro
  - [x] Compra das peças
  - [x] Montagem do aro
    - [ ] Adicionar couro nas laterais do aro
  - [x] Programação do aro
    - [x] Botoes
      - [x] 12 botões
      - [x] 2 botões de troca de marcha
    - [x] Led de status
      - [x] Ligado
      - [x] Conexão Bluetooth
    - [x] Botão On / Off
    - [ ] Indicador de porcentagem de bateria
  - [x] Testes e ajustes
- [x] Pedais
  - [x] Impressão 3D dos pedais
  - [x] Compra das peças
  - [x] Montagem dos pedais
  - [x] Programação dos pedais
  - [x] Testes e ajustes

---

## O que é?

Sim racing é uma competição de automobilismo virtual, onde os pilotos simulam corridas em ambiente digital usando equipamentos que imitam a sensação de dirigir um carro de verdade. Estes simuladores são amplamente usados tanto para entretenimento quanto para treinamento profissional, buscando replicar uma experiência de corrida realista com respostas sensoriais e feedback tátil.

## Objetivo do repositório

Este projeto visa construir um conjunto de equipamentos para simulação de corrida, incluindo um volante com force feedback, um aro com controles avançados e pedais personalizados. O repositório documenta o processo de construção, programação e configuração, permitindo que outros possam replicar ou modificar o setup para suas próprias necessidades.

## Referencias

Para aprofundamento sobre simulação de corrida e construção de volantes e pedais, consulte:

- [FFBeast](https://ffbeast.github.io/docs/en/wheel.html)
- [A Chave Virou](https://www.youtube.com/watch?v=a1Q18tryYvI)
  - Excelente video sobre a construção do volante + force feedback

---

## Volante + force feedback

### Descrição

O volante é a peça principal para controle de direção e sensação de pilotagem. Nele, o force feedback proporciona uma experiência realista ao simular as forças que agem sobre um carro, como curvas acentuadas, frenagens bruscas, e mudanças de terreno.

### Imagens e Videos

🚧 Em breve

### Bibliotecas utilizadas

- [FFBeast](https://ffbeast.github.io/)
  - Utilizado para a criação do force feedback

### Funcionalidades

- Force feedback com ajuste de intensidade.
- Centralização automática.
- Indicador de status (ligado/desligado).

### Modelos 3D

- [Case estilo fanatec](https://cults3d.com/en/3d-model/game/fanatec-style-case-for-hoverboard-sim-wheel)
  - Suporte para o motor de hoverboard e componentes eletrônicos
- [Prensa de mesa](https://cults3d.com/en/3d-model/game/table-clamp-for-fanatec)
  - Fixação segura do volante à mesa

### Componentes e ferramentas

- Motor de hoverboard
  - Utilizado para criar a sensação de resistência e feedback.
- [ODESC 4.2](https://pt.aliexpress.com/item/1005005763345874.html)
  - Controlador para gerenciamento do motor e da intensidade do force feedback junto ao FFBEAST.
- [1x Botão momentaneo](https://pt.aliexpress.com/item/1005005904666959.html)
  - Botão utilizado para centralizar o volante
- [Resistor de frenagem 50w 10ohm](https://pt.aliexpress.com/item/1005006825516125.html)
  - Necessário para dissipação de energia durante o uso intensivo.
- [Fonte 12v 30a](https://produto.mercadolivre.com.br/MLB-3732855248-fonte-24v-30a-720w-chaveada-estabilizada-bivolt-cabo-_JM)
  - Fornece energia suficiente para o funcionamento do motor.
- [Encoder Rotativo 1000ppr](https://pt.aliexpress.com/item/1005005779039462.html)
  - Responsável pela precisão na leitura da rotação do volante
- [2x Cooler 24v](https://pt.aliexpress.com/item/1005002468157849.html)
  - Utilizado para resfriamento do motor

### Esquema Elétrico

Em Breve.

---

## Aro do volante

### Descrição

O aro é a parte externa que o piloto segura e onde estão dispostos os controles de troca de marcha e outros botões personalizáveis. Conecta-se ao volante principal e comunica-se com o computador, permitindo interação com o jogo e ajustes durante a corrida.

Utilize a programação da pasta `volante_botoes` para a programação do ESP32.

### Imagens e Videos

🚧 Em breve

### Bibliotecas utilizadas

- [ESP32-BLE-Gamepad](https://github.com/lemmingDev/ESP32-BLE-Gamepad/)
  - Biblioteca utilizada no ESP32 para transformar a placa em um controle de jogos via bluetooth
- [Bounce2](https://github.com/thomasfredericks/Bounce2)
  - Biblioteca utilizada para a leitura de botões com debounce evitando multiplos acionamentos
- [TickTwo](https://github.com/sstaub/TickTwo)
  - Biblioteca utilizada para a criação de timers
  - Utilizada para a criação de um timer para controlar a `piscada` do led On/Off e de conexão

### Funcionalidades

- Conexão via Bluetooth ou USB-C.
- Troca de marcha, controle de tração e frenagem, pausa e outras ações.
- 12 botões personalizáveis.
- LEDs indicadores de status (ligado/conectado).

### Modelos 3D

- [Aro do volante F1](https://www.thingiverse.com/thing:4891321)
  - Para impressão do aro.
- [Saque rapido / Quick Release](https://www.thingiverse.com/thing:2749233)
  - Para troca rápida do aro.

### Componentes e ferramentas

- [ESP32 devkit](https://pt.aliexpress.com/item/1005004605399313.html)
  - Responsável pela comunicação via Bluetooth e detecção de comandos do aro.
- [ENDER 3 v3 KE](https://www.creality.com/products/creality-ender-3-v3-ke)
- [12x Botão momentaneo](https://pt.aliexpress.com/item/1005005904666959.html)
  - Configurados para funções específicas no jogo (ex. troca de marcha, menu).
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

### Esquema Elétrico

Em Breve

---

## Pedais

### Descrição

Os pedais são responsáveis por simular o acelerador, freio e (opcionalmente) embreagem. Atualmente, o projeto inclui apenas acelerador e freio, pois a troca de marcha será feita no volante.

Utilize a programação da pasta `pedals_joystick` para a programação do Arduino Pro Micro.

### Imagens e Videos

🚧 Em breve

### Bibliotecas utilizadas

- [SimRacing](https://github.com/dmadison/Sim-Racing-Arduino)
  - Biblioteca utilizada para a comunicação entre os pedais e o computador
- [Joystick](https://github.com/MHeironimus/ArduinoJoystickLibrary)
  - É uma dependencia da biblioteca `SimRacing`
- [Bounce2](https://github.com/thomasfredericks/Bounce2)
  - Biblioteca utilizada para a leitura de botões com debounce evitando múltiplos acionamentos

### Funcionalidades

- Acelerador e freio com calibração de mínima e máxima.
- Indicador de status de calibração via LED.
- Sensibilidade dos pedais ajustável.

### Modelos 3D

- [SIM RACING PEDAL](https://cults3d.com/en/3d-model/game/sim-racing-pedal)
  - Utilizado para a impressão dos pedais
  - Acelerador
  - Freio

### Componentes e ferramentas

- [Arduino Pro Micro Type C](https://pt.aliexpress.com/item/1005006654641959.html)
  - Controla a comunicação dos pedais com o computador.
- [ENDER 3 v3 KE](https://www.creality.com/products/creality-ender-3-v3-ke)
- [Botao momentaneo](https://pt.aliexpress.com/item/1005005904666959.html)
  - Botão utilizado para calibrar o minimo e o maximo dos pedais
- [2x Molas de rodoar de caminhao](https://produto.mercadolivre.com.br/MLB-1501615041-mola-cromada-rodoar-rodo-ar-caminhao-15-unidades-_JM)
  - Responsáveis pelo retorno dos pedais à posição inicial.
- [2x Sensor hall 49e](https://pt.aliexpress.com/item/32852539663.html)
  - Sensor utilizado para a leitura das posições dos pedais
- [2x Imãs de neodimino 8x5mm](https://produto.mercadolivre.com.br/MLB-1463589800-im-neodimio-8x5-mm-n35-10-unidades-_JM)
  - Usados para detectar a posição exata dos pedais.
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

### Esquema Elétrico

Em breve.

---

## Contribuição

Este projeto é mantido como um trabalho em andamento e todas as contribuições são bem-vindas. Se você tiver ideias de melhorias, correções ou novos recursos, fique à vontade para abrir uma issue ou enviar um pull request. Certifique-se de seguir as diretrizes de contribuição e o padrão de codificação definidos para facilitar o processo de revisão.

Para contribuir, você pode:

1. Fazer um fork do repositório.
2. Criar uma nova branch para sua funcionalidade ou correção: `git checkout -b minha-nova-funcionalidade`
3. Fazer commit das suas alterações: `git commit -m 'Adiciona nova funcionalidade'`
4. Enviar para a branch principal: `git push origin minha-nova-funcionalidade`
5. Criar uma solicitação de pull request.

## Licença

Este projeto está licenciado sob a MIT License. Consulte o arquivo LICENSE para mais detalhes.

---

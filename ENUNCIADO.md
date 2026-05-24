Programação I
Licenciatura em Redes e Sistemas de computadores
Enunciado do Trabalho Prático
Sistema Mini NOC para Monitorização da Rede de uma PME
Este trabalho tem como principal objetivo criar uma aplicação para apoiar a monitorização e a manutenção da rede informática de uma pequena ou média empresa (PME). A aplicação deve permitir gerir equipamentos de rede, executar testes simples de conectividade, interpretar respostas simuladas de sensores, registar incidentes técnicos e manter um histórico das configurações aplicadas aos equipamentos.
No desenvolvimento do trabalho deverá ser demonstrada a correta utilização das estruturas de dados lecionadas na unidade curricular, nomeadamente: listas, filas e pilhas. Deverá ainda ser demonstrada a utilização de ficheiros de texto e ficheiros binários.
A aplicação não deverá depender de uma base de dados externa nem de um endpoint HTTP. Os dados persistentes da aplicação devem ser guardados em ficheiros binários e os dados de monitorização, comandos de rede, sensores e relatórios devem ser guardados ou importados através de ficheiros de texto.
A aplicação deve suportar a gestão de seis módulos principais: inventário de equipamentos, testes de conectividade, monitorização de sensores, incidentes técnicos, registo de configurações e relatórios técnicos.

Módulo 1 — Inventário de Equipamentos da Rede
A PME dispõe de uma rede informática composta por equipamentos como router, switch, access points, servidor ou NAS, impressoras de rede, câmaras IP, sensores e UPS. Sobre cada equipamento a aplicação deve armazenar: código interno único, atribuído automaticamente, nome do equipamento, tipo de equipamento, marca, modelo, endereço IP, endereço MAC, localização física, estado operacional e data da última verificação. Sobre os equipamentos, a aplicação deve permitir:

Adicionar um novo equipamento de rede;
Remover um equipamento, desde que não tenha incidentes técnicos pendentes;
Alterar os dados de um equipamento;
Alterar o estado de um equipamento para Operacional, Em Falha, Em Manutenção ou Desativado;
Listar todos os equipamentos;
Listar equipamentos por tipo;
Listar equipamentos por estado;
Pesquisar equipamento por código, endereço IP ou endereço MAC;
Guardar e carregar o inventário de equipamentos através de ficheiro binário;
Outras atividades que considere relevantes.


Módulo 2 — Testes de Conectividade e Comandos de Rede
A aplicação deve permitir executar testes simples de conectividade sobre os equipamentos registados. O teste obrigatório é o comando ping, executado a partir do programa em C. O programa deve guardar o resultado bruto do comando num ficheiro de texto e, de seguida, analisar esse ficheiro para decidir se o equipamento respondeu ou não.
O trabalho poderá ser desenvolvido e testado em Windows ou Linux. O sistema operativo utilizado deve ser indicado no relatório do trabalho prático. Os comandos seguintes servem como referência:
Comandos de referência em Windows
ping -n 4 <IP> > resultado_ping.txt
ipconfig > resultado_rede_local.txt
arp -a > resultado_arp.txt
nslookup <dominio> > resultado_dns.txt
tracert <IP ou dominio> > resultado_rota.txt
Comandos de referência em Linux
ping -c 4 <IP> > resultado_ping.txt
ip addr > resultado_rede_local.txt
arp -a > resultado_arp.txt
nslookup <dominio> > resultado_dns.txt
traceroute <IP ou dominio> > resultado_rota.txt
Como requisito mínimo do trabalho, apenas o comando ping tem de ser executado e analisado. Os restantes comandos podem ser utilizados como funcionalidades extra.
Relativamente aos testes de conectividade, a aplicação deve permitir:

Selecionar um equipamento registado e executar um teste ping ao seu endereço IP;
Guardar o resultado bruto do comando num ficheiro de texto;
Ler o ficheiro de resultado e determinar se o equipamento respondeu;
Atualizar automaticamente a data da última verificação do equipamento;
Alterar o estado do equipamento para "Em Falha" quando não existir resposta;
Registar cada teste realizado num ficheiro de texto denominado <log_monitorizacao.txt>;
Criar automaticamente um incidente técnico quando um equipamento não responde;
Permitir um teste geral da rede, executando ping a todos os equipamentos registados;
Outras atividades que considere relevantes.


Módulo 3 — Monitorização de Sensores
A PME possui uma rack técnica equipada com sensores de temperatura, humidade e monitorização do estado da UPS. Para simplificar o desenvolvimento do trabalho, estes dados não serão obtidos através de um endpoint externo. Em vez disso, a aplicação deverá importar as leituras a partir de um ficheiro de texto, simulando os dados emitidos pelos sensores IoT.
O ficheiro de entrada deve denominar-se <sensores_rack.txt> e cada linha deve seguir o seguinte formato:
codigo_sensor;tipo;valor;unidade;estado
Exemplo de conteúdo do ficheiro:
TEMP_RACK;Temperatura da rack;38.5;C;CRITICO
HUM_RACK;Humidade da rack;72;%;AVISO
UPS_BAT;Bateria da UPS;18;%;CRITICO
UPS_ENERGIA;Estado da energia;0;-;FALHA_REDE
Sobre as leituras dos sensores, a aplicação deve permitir:

Importar leituras a partir do ficheiro <sensores_rack.txt>;
Guardar as leituras importadas na estrutura de dados definida pelo aluno;
Listar as leituras mais recentes;
Pesquisar leituras por código de sensor;
Identificar leituras com estado AVISO, CRITICO ou FALHA_REDE;
Criar automaticamente um incidente técnico sempre que for importada uma leitura em estado AVISO, CRITICO ou FALHA_REDE;
Registar a importação das leituras num ficheiro de texto denominado <log_sensores.txt>;
Outras atividades que considere relevantes.


Módulo 4 — Incidentes Técnicos e Fila de Atendimento
Sempre que um equipamento não responde ao teste ping ou uma leitura de sensor apresenta estado anómalo, deve ser criado um incidente técnico. A cada incidente é atribuído um número sequencial e são registados o código do equipamento ou sensor associado, o tipo de incidente, a descrição, a prioridade, a data e hora de criação, o técnico responsável e o estado (Em Curso, Pendente e Concluido). Sobre os incidentes, a aplicação deve permitir:

Criar um novo incidente manualmente;
Criar um incidente automaticamente a partir de falha no ping;
Criar um incidente automaticamente a partir de leitura anómala de sensor;
Colocar os incidentes pendentes numa fila de atendimento;
Processar o próximo incidente da fila, alterando o estado para Em Curso;
Concluir um incidente, registando a data e hora de conclusão;
Listar todos os incidentes pendentes;
Listar todos os incidentes em curso;
Listar todos os incidentes concluídos;
Listar incidentes por equipamento ou sensor;
Listar incidentes por prioridade;
Guardar e carregar os incidentes através de ficheiro binário;
Outras atividades que considere relevantes.


Módulo 5 — Registo de Configurações e Pilha de Reversão
Sempre que é aplicada uma configuração a um equipamento de rede, o registo dessa alteração deve ser guardado, permitindo consultar e reverter as alterações mais recentes. Cada registo deve conter: o código do equipamento configurado, o tipo de configuração, o valor anterior, o novo valor aplicado, a data e hora da operação e o técnico responsável. Sobre o registo de configurações, a aplicação deve permitir:

Registar uma nova configuração de um equipamento;
Consultar a última configuração realizada;
Consultar as N configurações mais recentes;
Reverter a última configuração aplicada, restaurando o valor anterior no registo do equipamento;
Consultar o histórico de configurações associadas a um equipamento;
Guardar e carregar o histórico de configurações através de ficheiro binário;
Limpar o registo de configurações, mediante confirmação do utilizador;
Outras atividades que considere relevantes.


Módulo 6 — Ficheiros e Relatórios
A aplicação deve utilizar ficheiros binários para armazenar os dados principais e ficheiros de texto para entradas, logs e relatórios. A utilização dos ficheiros deve ser integrada no funcionamento normal da aplicação, e não apenas executada no final do trabalho.
Ficheiros binários mínimos esperados:
equipamentos.dat
incidentes.dat
configuracoes.dat
leituras_sensores.dat
Ficheiros de texto mínimos esperados:
resultado_ping.txt
sensores_rack.txt
log_monitorizacao.txt
log_sensores.txt
relatorio_estado_rede_mes_ano.txt
Sobre ficheiros e relatórios, a aplicação deve permitir:

Carregar os dados existentes ao iniciar a aplicação;
Guardar os dados actualizados antes de sair da aplicação;
Importar leituras de sensores a partir de ficheiro de texto;
Guardar os resultados dos comandos de rede em ficheiros de texto;
Gerar um relatório de estado da rede, listando equipamentos operacionais, equipamentos em falha, incidentes pendentes e leituras anómalas;
Gerar um relatório mensal de incidentes, com totais por tipo de incidente e por prioridade;
Outras atividades que considere relevantes.


Requisitos mínimos
O trabalho deverá demonstrar, no mínimo, o seguinte fluxo de utilização:

Iniciar a aplicação e carregar os dados existentes dos ficheiros binários;
Registar, pelo menos, cinco equipamentos da rede da PME;
Executar um teste ping a um equipamento escolhido pelo utilizador;
Guardar o resultado do ping num ficheiro de texto;
Ler o ficheiro de resultado e indicar se o equipamento respondeu ou não respondeu;
Criar automaticamente um incidente quando o equipamento não responde;
Importar leituras simuladas de sensores a partir do ficheiro <sensores_rack.txt>;
Criar automaticamente incidentes para leituras em estado AVISO, CRITICO ou FALHA_REDE;
Processar o próximo incidente da fila de atendimento;
Concluir um incidente;
Registar uma alteração de configuração num equipamento;
Reverter a última alteração de configuração através da pilha;
Gerar um relatório de estado da rede num ficheiro de texto;
Guardar os dados actualizados nos ficheiros binários antes de sair.

A utilização do sistema de controlo de versões Git é obrigatória. O repositório deverá evidenciar a realização de commits regulares, permitindo verificar o contributo individual de cada elemento do grupo.

Trabalho extra
O desenvolvimento de funcionalidades extra poderá contribuir para a valorização da nota final, sendo atribuídos até 2 valores pela qualidade e relevância dessas funcionalidades. Exemplos de funcionalidades extra possíveis:

Suportar automaticamente comandos diferentes para Windows e Linux;
Extrair estatísticas do ping, como percentagem de perda de pacotes ou tempo médio de resposta;
Executar e guardar resultados de nslookup, arp, ipconfig, ip addr, tracert ou traceroute;
Ordenar equipamentos por estado, tipo ou localização;
Criar um resumo textual do estado da rede, indicando se a PME está em estado Normal, Aviso ou Crítico;
Implementar autenticação simples de técnico, sem utilização de base de dados externa.

Bom trabalho!
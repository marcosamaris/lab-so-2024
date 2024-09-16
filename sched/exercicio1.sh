#!/bin/bash
echo "Digite o PID do processo a ser alterado:"
read PID
echo "Digite o tipo de escalonamento (0: SCHED_OTHER, 1: SCHED_FIFO, 2: SCHED_RR, 3: SCHED_BATCH):"
read POLICY

chrt -p $POLICY $PID
echo "Política de escalonamento do processo $PID alterada."

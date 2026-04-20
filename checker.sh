#!/bin/bash

# Couleurs pour la lecture
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Fonction de test
run_test() {
    local label=$1
    local cmd=$2

    # Exécute la commande dans les deux shells
    echo -n "$cmd" | ./hsh > out_hsh 2> err_hsh
    echo -n "$cmd" | sh > out_sh 2> err_sh

    # Compare les sorties standard (stdout)
    diff out_hsh out_sh > /dev/null
    RES_OUT=$?

    if [ $RES_OUT -eq 0 ]; then
        echo -e "${GREEN}[PASS]${NC} $label"
    else
        echo -e "${RED}[FAIL]${NC} $label (Sortie différente)"
        echo "--- Attendue (sh) ---"
        cat out_sh
        echo "--- Reçue (hsh) ---"
        cat out_hsh
    fi
}

echo "--- Démarrage des tests du Simple Shell ---"

# 1. Tests de base
run_test "Commande simple (ls)" "ls"
run_test "Arguments multiples" "ls -l /var"
run_test "Chemin absolu" "/bin/ls"

# 2. Gestion des espaces (Edge Cases)
run_test "Espaces au début" "    ls"
run_test "Espaces à la fin" "ls    "
run_test "Espaces multiples" "ls      -la"

# 3. Built-ins
run_test "Built-in env" "env"

# 4. Commandes inexistantes
run_test "Commande inconnue" "not_a_command"

# 5. Entrées vides
run_test "Entrée vide (Entrée)" ""
run_test "Espaces uniquement" "      "

echo "--- Fin des tests ---"
rm out_hsh out_sh err_hsh err_sh

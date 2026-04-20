#!/bin/bash
# Checker Simple Shell - version isolée
# Corrige les faux négatifs :
#  - tests lancés dans un dossier propre (fin des collisions out_sh/out_hsh)
#  - env comparé par code de retour uniquement (sh modifie l'env, pas nous)
#  - sorties gardées en mémoire, rien n'est écrit dans le dossier de test

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Chemin absolu vers hsh avant de changer de dossier
HSH="$(pwd)/hsh"
if [ ! -x "$HSH" ]; then
    echo -e "${RED}Erreur : $HSH introuvable. Compile d'abord :${NC}"
    echo "    gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh"
    exit 1
fi

# Dossier de test isolé avec quelques fichiers connus pour ls
TESTDIR=$(mktemp -d)
touch "$TESTDIR/alpha" "$TESTDIR/beta" "$TESTDIR/gamma"
cd "$TESTDIR" || exit 1

PASS=0
FAIL=0

# Compare hsh et sh sur stdout
run_diff() {
    local label=$1
    local cmd=$2
    local out_h out_s

    out_h=$(echo -n "$cmd" | "$HSH" 2>/dev/null)
    out_s=$(echo -n "$cmd" | sh    2>/dev/null)

    if [ "$out_h" = "$out_s" ]; then
        echo -e "${GREEN}[PASS]${NC} $label"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $label"
        echo "  --- Attendu (sh) ---"
        echo "$out_s" | sed 's/^/  /'
        echo "  --- Reçu   (hsh) ---"
        echo "$out_h" | sed 's/^/  /'
        FAIL=$((FAIL + 1))
    fi
}

# Vérifie seulement le code de retour de hsh
run_rc() {
    local label=$1
    local cmd=$2
    local want=$3

    echo -n "$cmd" | "$HSH" >/dev/null 2>/dev/null
    local rc=$?

    if [ "$rc" -eq "$want" ]; then
        echo -e "${GREEN}[PASS]${NC} $label (code=$rc)"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $label (attendu $want, reçu $rc)"
        FAIL=$((FAIL + 1))
    fi
}

# Vérifie que hsh sort du texte non vide avec le code attendu
run_nonempty() {
    local label=$1
    local cmd=$2
    local want=$3

    local out
    out=$(echo -n "$cmd" | "$HSH" 2>/dev/null)
    local rc=$?

    if [ -n "$out" ] && [ "$rc" -eq "$want" ]; then
        echo -e "${GREEN}[PASS]${NC} $label"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}[FAIL]${NC} $label (sortie vide ou code $rc != $want)"
        FAIL=$((FAIL + 1))
    fi
}

echo "--- Tests Simple Shell ---"
echo "Dossier de test : $TESTDIR"
echo

# 1. Commandes de base
run_diff     "ls dans dossier propre"       "ls"
run_diff     "ls avec argument"             "ls /var"
run_diff     "Chemin absolu"                "/bin/ls /etc"

# 2. Espaces
run_diff     "Espaces au début"             "    ls"
run_diff     "Espaces à la fin"             "ls    "
run_diff     "Espaces multiples"            "ls      /var"

# 3. Built-in env (sh modifie l'env, non comparable)
run_nonempty "Built-in env (sortie, rc=0)"  "env" 0

# 4. Commande inconnue
run_rc       "Commande inconnue (rc=127)"   "not_a_command" 127

# 5. Entrées vides
run_diff     "Entrée vide"                  ""
run_diff     "Espaces uniquement"           "      "

echo
echo -e "--- Résultat : ${GREEN}${PASS} PASS${NC} / ${RED}${FAIL} FAIL${NC} ---"

cd - >/dev/null
rm -rf "$TESTDIR"

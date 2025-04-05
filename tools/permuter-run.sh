INTERRUPTED=0
trap 'INTERRUPTED=1' INT

source .push_secrets

IDENTIFIER="${1#nonmatchings\/}"
IDENTIFIER="${IDENTIFIER%/}"

python3 tools/decomp-permuter/permuter.py \
  -j 16 \
  --best-only \
  --stop-on-zero \
  --stack-diffs \
  --algorithm levenshtein $1

if [ $INTERRUPTED -eq 0 ] && [ $? -eq 0 ]; then
  curl -s \
    --form-string "token=$PUSH_TOKEN" \
    --form-string "user=$PUSH_USER" \
    --form-string "message=Matched $IDENTIFIER" \
    https://api.pushover.net/1/messages.json
fi

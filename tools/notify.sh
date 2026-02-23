#!/bin/bash

# Send a push notification via Pushover
# Usage: ./notify.sh <message>

if [ $# -eq 0 ]; then
    echo "Usage: $0 <message>"
    exit 1
fi

MESSAGE="$1"

if [ -f .push_secrets ]; then
    source .push_secrets
    curl -s \
        --form-string "token=$PUSH_TOKEN" \
        --form-string "user=$PUSH_USER" \
        --form-string "message=$MESSAGE" \
        https://api.pushover.net/1/messages.json
else
    echo "Error: .push_secrets file not found"
    exit 1
fi

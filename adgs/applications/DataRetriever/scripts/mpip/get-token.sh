#!/bin/bash -e

. auth.env.sh

CONTENT_TYPE='application/x-www-form-urlencoded'
TOKEN_RESPONSE=token-response.json
ACCESS_TOKEN=access-token.txt

curl -v -k -H "Host: $AUTH_HOST" -H "Content-Type: $CONTENT_TYPE" -d client_id=$CLIENT_ID -d client_secret=$CLIENT_SECRET -d username=$AUTH_USER -d password=$AUTH_PASS -d grant_type=password $AUTH_URL | tee $TOKEN_RESPONSE
#curl -v -f -k -u "$CLIENT_ID:$CLIENT_SECRET" -d "username=$AUTH_USER&password=$AUTH_PASS&grant_type=password" $AUTH_URL

cat $TOKEN_RESPONSE | jq .access_token | tr -d '"' > $ACCESS_TOKEN

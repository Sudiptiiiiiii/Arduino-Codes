import requests
import sys
endpoint = 'http://127.0.0.1:8000/api/transaction/'

card_number = sys.argv[1]
item_retrieved = sys.argv[2]
number_of_items = sys.argv[3]


data = {
    'Card_Number': card_number,
    'Item_Retrieved': item_retrieved,
    'Number_of_Items': number_of_items,
}

response = requests.post(endpoint, json=data)

if response.status_code == 201:
    print(response)
    print('Post request successful')
else:
    print(response)
    print('Post request failed')
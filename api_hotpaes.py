from flask import Flask, request, json

app = Flask(__name__)

api_header_key =  "ueyr123768565HGHgjhgHGHJghjghgHDFgdfdhgfklkjlkjuytty68"
api_key_post =  "1A2b3C4E5f"

@app.route('/data_temp', methods=['GET','POST'])
def data_temp():
    if request.headers.get('Authorization-Token') != api_header_key:
        return json.dumps({'data_temp':'Erro de authenticação Header'})
    else:
        if request.form.get('api_key') != api_key_post:
            # falha da autenticação do cabeçalho
            return json.dumps({'data_temp':'Erro de authenticação modulo'} , ensure_ascii=False )

        else:
            status_id=request.form.get('id')
            status_temp=request.form.get('medida')
            status_umidade=request.form.get('umidade_pao')
            status_frase=request.form.get('frase')

            return json.dumps({'id': status_id, 'temperatura': status_temp, 'umidade_pao': status_umidade, 'frase': status_frase})
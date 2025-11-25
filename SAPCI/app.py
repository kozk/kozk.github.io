from flask import Flask, render_template, request
import datetime
from pymongo import MongoClient

app = Flask(__name__)
cliente = MongoClient("mongodb+srv://itzcui_db_user:8Zu4Fik8n6OmwHCw@cdmx.k0skwr4.mongodb.net/?appName=CDMX")
app.db = cliente.blog

entradas = [entrada for entrada in app.db.contenido.find({})]
print(entradas)

@app.route("/", methods = ["GET", "POST"])
def home():
    if request.method == "POST":
        titulo = request.form.get("tit")
        contenido_entrante = request.form.get("content")
        fecha_formato = datetime.datetime.today().strftime("%d -%m-%Y")
        parametros = {"titulo": titulo, "contenido": contenido_entrante, "fecha": fecha_formato}
        entradas.append(parametros)
        app.db.contenido.insert_one(parametros)

    return render_template("index.html", entradas = entradas)

if __name__ == "__main__":
    app.run()
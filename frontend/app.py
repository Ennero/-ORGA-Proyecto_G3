from flask import Flask, render_template, request, jsonify

app = Flask(__name__)

@app.route("/", methods=["GET"])
def index():
    return render_template("index.html")

@app.route("/procesar", methods=["POST"])
def procesar():
    data = request.get_json()
    texto = data.get("texto", "")
    
    # Aquí podrías procesar el texto como desees, por ahora solo lo devolvemos
    resultado = f"Juego procesado con el siguiente contenido:\n{texto}"
    
    return jsonify({"resultado": resultado})


@app.route("/configuracion", methods=["GET"])
def configuracion():
    return render_template("configuracion.html")

@app.route("/iniciarJuego", methods=["GET"])
def iniciar_juego():
    return render_template("iniciarJuego.html")


if __name__ == "__main__":
    app.run(debug=True)



import datetime

from re import template
from turtle import update
from typing import Union
from urllib import response
from fastapi import FastAPI,Request
from fastapi.responses import HTMLResponse
from fastapi.templating import Jinja2Templates
from fastapi.staticfiles import StaticFiles
import firebase_admin
from firebase_admin import credentials
from firebase_admin import db

app = FastAPI()
template = Jinja2Templates(directory="templates")
app.mount("/static", StaticFiles(directory="static"), name="static") 

@app.get("/",response_class=HTMLResponse)
def read_root(request: Request):
    ref=db.reference(f"/temp")
    temp=ref.get()
    ref=db.reference(f"/humidity")
    humidity=ref.get()
    ref=db.reference(f"/control")
    control=ref.get()
    return template.TemplateResponse("./index.html",{"request":request,"temps":temp,"humidity":humidity,"control":control})
    

@app.get("/temp/{temps}",response_class=HTMLResponse)
def read_t(request: Request,temps: int):
    ref = db.reference(f"/temp")
    ref.set(f"{temps}")
    
    return template.TemplateResponse("./temp.html",{"request":request,"temps":temps})

@app.get("/humidity/{humiditys}/temp/{temps}",response_class=HTMLResponse)
def read_h(request: Request,humiditys: int,temps: int):
    ref = db.reference(f"/humidity")
    ref.set(f"{humiditys}")
    ref = db.reference(f"/temp")
    ref.set(f"{temps}")
    ref=db.reference(f"/control")
    control=ref.get()
    return template.TemplateResponse("./index.html",{"request":request,"humidity":humiditys,"temps":temps,"control":control})

@app.get("/total/{control}",response_class=HTMLResponse)
def read_total(request: Request,control:int):
    ref=db.reference(f"/control")
    ref.set(f"{control}")
    ref=db.reference(f"/temp")
    temp=ref.get()
    ref=db.reference(f"/humidity")
    humidity=ref.get()
    return template.TemplateResponse("./totalcontrol.html",{"request":request,"temp":temp,"humidity":humidity})
@app.get("/date",response_class=HTMLResponse)
def read_total(request: Request):
    ref=db.reference(f"/control")
    control=ref.get()
    ref=db.reference(f"/temp")
    temp=ref.get()
    ref=db.reference(f"/humidity")
    humidity=ref.get()
    return template.TemplateResponse("./index.html",{"request":request,"temps":temp,"humidity":humidity,"control":control})
@app.get("/control",response_class=HTMLResponse)
def read_total(request: Request):
    ref=db.reference(f"/control")
    control=ref.get()
    return template.TemplateResponse("./control.html",{"request":request,"control":control})

import firebase_admin
from firebase_admin import credentials

cred = credentials.Certificate("waterdht11server-firebase-adminsdk-5stoq-cd75d86eb9.json")
firebase_admin.initialize_app(cred,{
    'databaseURL' : 'https://waterdht11server-default-rtdb.firebaseio.com/'
})


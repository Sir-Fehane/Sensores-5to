from django.urls import path
from . import views
#from .views import SendNumberView

urlpatterns = [
    path('data/', views.send_number, name='send_number'),
    #path('recibir/', views.recibir_datos, name='recibir_datos'),
    #path('send/', views.mandar_number, name='mandar_number'),
    path('start/', views.start_cicle, name='start_cicle')
]
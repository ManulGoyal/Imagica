from django.views.generic import TemplateView,ListView,CreateView, View
from .models import FILE
from django.shortcuts import render
from .forms import EnterImg
from django.urls import reverse_lazy
import os,sys

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

class HomePgView(TemplateView):
    # model = FILE
    template_name = 'base.html'

class EnterImgView(CreateView):
    model = FILE
    form_class = EnterImg
    template_name = 'enter.html'
    success_url = reverse_lazy('edit1')

class PreView(View):
    def get(self,request):
        obj = FILE.objects.order_by('-pk')[0]
        return render(request, 'edit1.html', {'obj' : obj})

class SharpView(View):
    def get(self, request):
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        outp = BASE_DIR+'/media/images/output.jpg'
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'3'
        os.system(loc1)
        return render(request, 'sharp.html',{'path':outp})
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
        return render(request, 'sharp.html',{'path':loc})

class EdgeDetectView(View):
    def get(self, request):
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        outp = BASE_DIR+'/media/images/output.jpg'
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'2'
        os.system(loc1)
        return render(request, 'edgedetect.html',{'path':loc})

class BoxBlurView(View):
    def get(self, request):
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        outp = BASE_DIR+'/media/images/output.jpg'
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'1'
        os.system(loc1)
        return render(request, 'boxblur.html',{'path':loc})

class EmbossView(View):
    def get(self, request):
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        outp = BASE_DIR+'/media/images/output.jpg'
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'4'
        os.system(loc1)
        return render(request, 'emboss.html',{'path':loc})

class GaussianBlurView(View):
    def get(self, request):
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        outp = BASE_DIR+'/media/images/output.jpg'
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'5'
        os.system(loc1)
        return render(request, 'gaussblur.html',{'path':loc})

class CompressView(View):
    def get(self, request):
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        outp = BASE_DIR+'/media/images/output.jpg'
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'2'
        os.system(loc1)
        return render(request, 'edgedetect.html',{'path':outp})


class ResizeView(TemplateView):
    template_name = 'resize.html'

class ResizedView(View):
    def get(self, request):
        bilx = request.GET.get('bilinx')
        bily = request.GET.get('biliny')
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        loc1 = BASE_DIR + '/static/interpolate ' + BASE_DIR + loc + ' ' + BASE_DIR + '/media/images/output.jpg ' + bilx + ' ' + bily + ' 0'
        os.system(loc1)
        loc2 = BASE_DIR + '/static/interpolate ' + BASE_DIR + loc + ' ' + BASE_DIR + '/media/images/output1.jpg ' + bilx + ' ' + bily + ' 1'
        os.system(loc2)
        return render(request,'resized.html', {'path':loc})


class RotateView(TemplateView):
    template_name = 'rotate.html'

class RotatedView(View):
    def get(self, request):
        degree = request.GET.get('deg')
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        loc1 = BASE_DIR + '/static/rotate ' + BASE_DIR + loc + ' ' + BASE_DIR + '/media/images/output.jpg ' + degree + ' 0'
        os.system(loc1)
        loc2 = BASE_DIR + '/static/rotate ' + BASE_DIR + loc + ' ' + BASE_DIR + '/media/images/output1.jpg ' + degree + ' 1'
        os.system(loc2)
        return render(request,'resized.html', {'path':loc})

class CompressView(TemplateView):
    template_name = 'compress.html'

class CompressedView(View):
    def get(self, request):
        qfactor = request.GET.get('q')
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        loc1 = BASE_DIR + '/static/encoder ' + BASE_DIR + loc + ' ' + BASE_DIR + '/media/images/output.imgc ' + qfactor 
        os.system(loc1)
        loc2 = BASE_DIR + '/static/decoder ' + BASE_DIR + '/media/images/output.imgc ' '1 ' + BASE_DIR + '/media/images/output.png'
        os.system(loc2)
        
        # f = open(BASE_DIR + '/media/images/psnr.txt', 'r')
        # file_contents = f.read()
        return render(request, 'compressed.html', {'path':loc,'loc1':loc1, 'loc2':loc2})

class CustomView(TemplateView):
    template_name = 'custom.html'

class CustomedView(View):
    def get(self, request):
        l=[]
        s=""
        obj = FILE.objects.order_by('-pk')[0]
        loc = obj.data.url
        for i in range(5):
            for j in range(5):
                l.append(request.GET.get('field'+str(i)+str(j)))
        s1 = [str(i) for i in l]
        s = ' '.join(s1)
        loc1 = BASE_DIR+ "/static/source "+BASE_DIR+loc+' '+ BASE_DIR+'/media/images/output.jpg ' +'6 '+s 
        os.system(loc1)
        return render(request, 'customed.html' , {'s':s,'path':loc })
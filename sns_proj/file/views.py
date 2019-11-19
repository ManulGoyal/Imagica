from django.views.generic import TemplateView,ListView, CreateView
from .models import FILE
from .forms import EnterImg
from django.urls import reverse_lazy

class HomePgView(TemplateView):
    # model = FILE
    template_name = 'base.html'

class EnterImgView(CreateView,ListView):
    model = FILE
    form_class = EnterImg
    template_name = 'enter.html'
    success_url = reverse_lazy('edit1')

class PreView(ListView):
    model = FILE
    template_name = 'edit1.html'
    # def get_context_object_name(self,object_list):
    #     return self.context_object_name

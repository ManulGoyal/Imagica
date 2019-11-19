from django import forms
from .models import FILE

class EnterImg(forms.ModelForm):
    class Meta: 
        model = FILE
        fields = ['title','data']

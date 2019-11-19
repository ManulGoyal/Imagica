from django.conf.urls import url

from .views import HomePgView, EnterImgView ,PreView

urlpatterns = [
    url(r'^$', HomePgView.as_view(), name = 'base'),
    url(r'^enter/$',EnterImgView.as_view(), name = 'Enter_image'),
    url(r'^edit1/$',PreView.as_view(),name='edit1'),
]
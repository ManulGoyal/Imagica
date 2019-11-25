from django.conf.urls import url
from .views import HomePgView, EnterImgView , PreView, SharpView, BoxBlurView, EmbossView , GaussianBlurView ,EdgeDetectView, ResizeView, ResizedView, RotatedView, RotateView, CompressView, CompressedView, CustomView, CustomedView

urlpatterns = [
    url(r'^$', HomePgView.as_view(), name = 'base'),
    url(r'^enter/$',EnterImgView.as_view(), name = 'enter'),
    url(r'^edit1/$',PreView.as_view(),name='edit1'),
    url(r'^edit1/sharp/$',SharpView.as_view(),name='sharpen'),
    url(r'^edit1/boxblur/$',BoxBlurView.as_view(),name='boxblur'),
    url(r'^edit1/emboss/$',EmbossView.as_view(),name='emboss'),
    url(r'^edit1/edgedetect/$',EdgeDetectView.as_view(),name='edgedetect'),
    url(r'^edit1/gaussblur/$',GaussianBlurView.as_view(),name='gaussblur'),
    url(r'^edit1/resize/$',ResizeView.as_view(),name='resize'),
    url(r'^edit1/resize/resized/$', ResizedView.as_view(), name='resized'),
    url(r'^edit1/rotate/$',RotateView.as_view(),name='rotate'),
    url(r'^edit1/rotate/rotated/$', RotatedView.as_view(), name='rotated'),
    url(r'^edit1/compress/$',CompressView.as_view(),name='compress'),
    url(r'^edit1/compress/compressed/$', CompressedView.as_view(), name='compressed'),  
    url(r'^edit1/custom/$',CustomView.as_view(),name='custom'),
    url(r'^edit1/custom/customed/$',CustomedView.as_view(),name='customed'),

]
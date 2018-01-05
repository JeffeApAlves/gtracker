django.jQuery(document).ready(function($) {
    $('#id_position_0, #id_position_1').attr('readonly', 'readonly');
   
    $('#id_rua, #id_cidade, #id_estado').blur(function(event) {
     /* Act on the event */
     if ($('#id_rua').val()!='' && $('#id_cidade').val()!='' && $('#id_estado').val()!='') {
      $('.geoposition-search input').val($('#id_rua').val()+' ' +$('#id_cidade').val()+' '+$('#id_estado').val());
      
      // TRIGGER DO ENTER PARA EXECUTAR A BUSCA
      var e = $.Event("keydown");
      e.which = 50; // # Some key code value
      $(".geoposition-search input").trigger(e);
     }
    });
   });
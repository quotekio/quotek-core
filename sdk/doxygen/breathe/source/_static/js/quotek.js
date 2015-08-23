function sendmail() {

  var sender = $('#sender').val();
  var subject = $('#subject').val();
  var message = $('#message').val();

  var smr = $.ajax({ url: '/async/actions',
                     type: 'POST',
                     data: { 'action': 'sendmail',
                     	     'sender': sender,
                             'subject': subject,
                             'message': message  } ,
                     async:true,
                     cache: false,
                     success: function() { alert('Messagge succesfully sent, we\'ll contact you back ASAP !');  }});

}